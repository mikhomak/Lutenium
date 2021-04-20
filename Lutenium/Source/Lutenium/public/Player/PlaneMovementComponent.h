
#pragma once

#include "CoreMinimal.h"
#include "MovementEffect/MovementEffect.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "PlaneMovementComponent.generated.h"


/**
 * Generic Plane movement component
 * Can work with any actor/pawn
 * Has 4 main Inputs:
 *      Thrust(W-S) - adds acceleration to the jet. ThrustInput()
 *      Pitch(UP-DOWN) - adds pitch torque to the jet. PitchInput()
 *      Roll(RIGHT-LEFT) - adds roll torque to the jet. RollInput()
 *      Yawn(A-D) - adds yawn torque to the jet. YawnInput()
 * Rotation inputs are not calculated during the tick, they should be bind with the input axis.
 * Thrust input has to bind with the input axis as well, however it's uses a timer to calculate the acceleration and adds it to the physics velocity in tick
 * ----------------------
 * ----------------------
 * ----------------------
 * Movement Effects:
 * PlaneMovementComponent can have Effects that could add aditional "movement effects" each tick
 * Currently there are two effects:
 *      1) UDragMomvementEffect - Dragging the player to a specific direction
 *      2) UEmpMovementEffect - Rotates the jet and applying stalling to the player
 * ----------------------
 * ----------------------
 * ----------------------
 * Stalling
 * Stalling - removes current acceleration and rotates the jet by a random rotation (see UEmpMovementEffect)
 * To exit stalling, the jet should get accelerate to a certain point (ExitStallAcceleration)
 * Needs physics component(UPrimitiveComponent with PhysicsEnabled) in order to work and a pawn
 *
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDotHasChangedSignature, float, Dot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKickInAccelerationSignature);


UCLASS(meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UPlaneMovementComponent : public UActorComponent
{
    GENERATED_BODY()



protected:
    virtual void BeginPlay() override;

public:

    // ------------------------------------------------------------------
    // General
    // ------------------------------------------------------------------

    /**
     * Owner pawn
     * Needs to be set after creating the component
     */
    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    class APawn* OwnerPawn;

    /**
     * Main component(Root component of the owner actor) of to aapply the forces to
     * Should have SimulatePhysics = true
     * Needs to be set after creating the component
     */
    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    class UPrimitiveComponent* PhysicsComponent;

    /**
     * Delta time
     * Updates in the TickComponent()
     */
    UPROPERTY(BlueprintReadWrite, Category = "General")
    float fDeltaTime;

    /** Constructor idk*/
    UPlaneMovementComponent();

    /**
     * In tick we apply thrust, movement effects, stalling and aerodynamics
     * Torque are applied in input methods that are bind with the input axises
     */
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;


    // ------------------------------------------------------------------
    // Inputs
    // ------------------------------------------------------------------

    /**
     * Thrust input W/D
     * Adds acceleration to the front vector
     * Uses Thrusting()
     * @warning In your pawn class bind this method to the thrust axis!
     */
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void ThrustInput(const FInputActionValue& Value);

    /**
     * Adds torque to the Y axis
     * Uses AddTorqueToThePlane()
     * @warning In your pawn class bind this method to the Pitch axis!
     */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void PitchInput(const FInputActionValue& Value);

    /**
     * Adds torque to the Z axis
     * Uses AddTorqueToThePlane()
     * @warning In your pawn class bind this method to the Yawn axis!
     */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void YawnInput(const FInputActionValue& Value);

    /**
     * Adds torque to the X axis
     * Uses AddTorqueToThePlane()
     * @warning In your pawn class bind this method to the Roll axis!
     */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void RollInput(const FInputActionValue& Value);



    // ------------------------------------------------------------------
    // Pawn setters
    // ------------------------------------------------------------------

    /**
     * You can use this setter in BP to set the physics component
     */
    FORCEINLINE UFUNCTION(Category = "Owner")
    void SetBox(class UPrimitiveComponent* PrimitiveComp){ PhysicsComponent = PrimitiveComp; }

    /**
     * You can use this setter in BP to set the owner pawn
     */
    FORCEINLINE UFUNCTION(Category = "Owner")
    void SetPawn(class APawn* Pawn){ OwnerPawn = Pawn; }


    // ------------------------------------------------------------------
    // Acceleration
    // ------------------------------------------------------------------


    /**
     * Current Acceleration of the plane
     * Acceleration is added to the physical velocity as ForwardVector * CurrentAcceleration
     * Clamped between MinAcceleration and MaxAcceleration
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Acceleration")
    float CurrentAcceleration;


    /*
     * Added acceleration when the thurst input is > 0
     * Adds to CurrentAcceleation
     * Used in Thrusting()
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Acceleration" )
    float ThrustUpAcceleration;


    /*
     * Added acceleration when the thurst input is < 0
     * Adds to CurrentAcceleation
     * This value should be negative
     * Used in Thrusting()
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Acceleration")
    float ThrustDownAcceleration;

    /*
     * Added acceleration when the thurst input is < 0
     * Adds to CurrentAcceleation
     * This value should be negative
     * Used in Thrusting()
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Acceleration")
    float NoThrustDeceleration;


    /**
     * Max current acceleration
     * @warning speed(that lerps with the velocity) can be higher than this, but acceleration can not!
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Acceleration")
    float MaxAcceleration;

    /*
     * Min current acceleration
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Acceleration")
    float MinAcceleration;

    /**
     * Returns current acceleration
     * Acceleration != Velocity!
     * @return CurrentAcceleration
     */
    FORCEINLINE UFUNCTION(Category ="Acceleration")
    float GetCurrentAcceleration() {return CurrentAcceleration;}

    /**
     * Sets the current acceleration to 0
     * Used in EMP movement effect
     */
    FORCEINLINE UFUNCTION(Category = "Acceleration")
    void ResetCurrentAcceleration() { CurrentAcceleration = 0;}

    /* 
     * If the current acceleration is more than MaxAcceleration, than we lerp the current acceleration with the MaxAcceleration using this value as a alpha for lerp
     * Basically lerping the value to the MaxAcceleration when it's beyond MaxAcceleration, without "hard" clamping
     * Can happen when dashing
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Acceleration")
    float MaxSpeedLerpAlpha;

    /**
     * Maximum acceleration the plane could get when the ThrustInput is 1
     * Not the same as MaxAcceleration!
     * This one is the Maximum that we can get only with Thrusting, while MaxAcceleration also works with custom inputs, such as dashes and so on
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Acceleration")
    float MaxThrustUpAcceleration;

    /**
     * Minimum acceleration
     * This value clamping the minimum acceleration, CurrentAcceleration can't get below this value!
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Acceleration")
    float MinThrustAcceleration;


    /**
     * To add the acceleartion, each tick we setting the PhysicalVelocity 
     * The value we add is the lerp value between CurrentPhysicalVelocity and ForwardVector * Speed
     * Speed is calculated with CurrentAcceleration, MaxAcceleration, MinAcceleration and MaxSpeedLerpAlpha
     * @see AddThrust
     * @see CurrentAcceleration
     * @warning bDeactivateThrust deactivates AddThrust() and thus deactivetes thrusting
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Control")
    float LerpVelocity;

    /**
     * The functionality is the same as LerpVelocity but this one is applied when there is no thurst(ThrustInput = 0)
     * Usually should be higher than LerpVelocity because that way the jet gets more control withour thrust
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Control")
    float LerpVelocityNoThrust;


    /**
     * Jet can be stalling
     * Change this value if you want it to stall
     * Stalling is basically removing all the force applied(Acceleration) from the jet, and thus it will fall down
     * The only way to stop stalling is to increase ThrustInput and thus increase the CurrentAcceleration
     * When the current Acceleartion is biger than ExitStallAcceleration, the jet stops stalling
     * @warning if true, Doesn't apply Accleration and Aerodynamics, however the controls are still working!
     * @see UEmpMovementEffect
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Control")
    bool bStalling;

    void AddAcceleration(float AddedAcceleration);

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Acceleration")
    float ExitStallAcceleration;



    // ------------------------------------------------------------------
    // MovementEffects
    // ------------------------------------------------------------------

    UPROPERTY(BlueprintReadOnly, Category = "Effects")
    class UDragMovementEffect* DragMovementEffect;

    UPROPERTY(BlueprintReadOnly,  Category = "Effects")
    class UEmpMovementEffect* EmpMovementEffect;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effects|Class")
    TSubclassOf<class UDragMovementEffect> DragMovementEffectClass;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Effects|Class")
    TSubclassOf<class UEmpMovementEffect> EmpMovementEffectClass;


    UPROPERTY(BlueprintAssignable, Category="Physics|Aerodynamics")
    FDotHasChangedSignature OnDotHasChanged;

    UPROPERTY(BlueprintAssignable, Category="Physics|Aerodynamics")
    FOnKickInAccelerationSignature OnKickinAcceleration;

protected:

    // ------------------------------------------------------------------
    // CONTROLS
    // ------------------------------------------------------------------
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Control")
    float AirControl;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Control")
    float YawnControl;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Control")
    float PitchControl;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Control")
    float NoThrustPitchControl;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Control")
    float RollControl;



    // ------------------------------------------------------------------
    // SPEED
    // ------------------------------------------------------------------


    // ------------------------------------------------------------------
    // KICK IN
    // ------------------------------------------------------------------

    /* If the speed is below this value, the acceleration "kicks in" faster*/
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed|KickIn")
    float MinAccelerationUntilKickIn;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed|KickIn")
    float MaxAccelerationUntilKickIn;

    /* If the speed is below this value, the acceleration "kicks in" faster*/
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed|KickIn")
    float KickInAddedAcceleration;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Speed|KickIn")
    bool bHasAppliedKickInAcceleration;

    float CurrentThrust;

    bool bThrustUp;

    bool bThrusting;

    void AddTorqueToThePlane(FVector Direction, float InputVal) const;

    virtual void Thrusting(float InputVal);

    void AddThrust(float DeltaTime) const;

    void CalculateAcceleration();

    /* Main movement method */
    void Movement(const float DeltaTime);



    // ------------------------------------------------------------------
    // PHYSICS
    // ------------------------------------------------------------------


    /* Gravity depends on the speed, the faster the plane is moving the less gravity is applied */
    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float CustomMaxGravity;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float CustomMinGravity;


    /* Aerodynamic is applied all the time */
    /* Aerodynamic vector =  Opposite velocity vector * Multiplier*/
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Control")
    float AerodynamicMultiplier;

    UPROPERTY(BlueprintReadOnly, Category = "Physics")
    float Dot;



    void AddGravityForce(float DeltaTime) const;

    /* Fires the event when Dot value changes drastically to enable VFX in blueprints */
    void HasDotChanged(float DotProduct);



    void CalculateAerodynamic(float DeltaTime);

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Aerodynamic")
    float DotThreshold;

    // ------------------------------------------------------------------
    // MovementEffects
    // ------------------------------------------------------------------
    TArray<UMovementEffect*> MovementEffects;

    // ------------------------------------------------------------------
    // DEBUG
    // ------------------------------------------------------------------
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
    bool bDeactivateThrust;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
    bool bDeactivateAerodynamics;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
    bool bDeactivateGravity;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
    bool bApplyAerodynamicsOnSpecificValue_DEBUG;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
    bool bScaleAerodynamicsWithSpeed_DEBUG;
};

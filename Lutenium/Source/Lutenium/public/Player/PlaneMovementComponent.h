
#pragma once

#include "CoreMinimal.h"
#include "MovementEffect/MovementEffect.h"
#include "Components/ActorComponent.h"
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
     *
     * @warning In your pawn class bind this method to the thrust axis!
     */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ThrustInput(float Val);

    /**
     * Adds torque to the Y axis
     * Uses AddTorqueToThePlane()
     * @warning In your pawn class bind this method to the Pitch axis!
     */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void PitchInput(float Val);

    /**
     * Adds torque to the Z axis
     * Uses AddTorqueToThePlane()
     * @warning In your pawn class bind this method to the Yawn axis!
     */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void YawnInput(float Val);

    /**
     * Adds torque to the X axis
     * Uses AddTorqueToThePlane()
     * @warning In your pawn class bind this method to the Roll axis!
     */
    UFUNCTION(BlueprintCallable, Category = "Input")
    void RollInput(float Val);



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
    // Movement
    // ------------------------------------------------------------------
    FORCEINLINE UFUNCTION(Category ="Speed")
    float GetCurrentAcceleration() {return CurrentAcceleration;}

    FORCEINLINE UFUNCTION(Category = "Speed")
    void ResetCurrentAcceleration() { CurrentAcceleration = 0;}


    UPROPERTY(Category = Control, BlueprintReadWrite, EditAnywhere)
    bool bStalling;

    // ------------------------------------------------------------------
    // SPEED
    // ------------------------------------------------------------------

    UPROPERTY(Category = Speed, EditDefaultsOnly, BlueprintReadOnly)
    float MaxSpeed;

    UPROPERTY(Category = Speed, EditDefaultsOnly, BlueprintReadOnly)
    float MinSpeed;

    /* Alpha to lerp the speed when the value is above MaxSpeed(could happen with dash) */
    UPROPERTY(Category = Speed, EditDefaultsOnly, BlueprintReadOnly)
    float MaxSpeedLerpAlpha;

    UPROPERTY(Category = Speed, BlueprintReadOnly, EditDefaultsOnly)
    float MaxThrustUpAcceleration;

    UPROPERTY(Category = Speed, BlueprintReadOnly, EditDefaultsOnly)
    float MaxThrustDownAcceleration;

    void AddAcceleration(float AddedAcceleration);

    UPROPERTY(Category = Speed, BlueprintReadWrite, EditDefaultsOnly)
    float ExitStallAcceleration;

    UPROPERTY(Category = Speed, BlueprintReadOnly)
    float CurrentAcceleration;
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

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Control")
    float LerpVelocity;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Control")
    float LerpVelocityNoThrust;

    // ------------------------------------------------------------------
    // SPEED
    // ------------------------------------------------------------------

    /* Added acceleration while thrusting */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed" )
    float ThrustUpAcceleration;

    /* Added acceleration while stopping */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
    float ThrustDownAcceleration;

    /* Deceleration while no thrust is applied */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
    float NoThrustDeceleration;

    /* If the speed is below this value, the acceleration "kicks in" faster*/
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
    float MaxAccelerationUntilTakeOff;

    /* If the speed is below this value, the acceleration "kicks in" faster*/
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
    float TakeOffAddedAcceleration;

    bool bHasAppliedTakeOffAcceleration;

    float CurrentThrust;

    bool bThrustUp;

    bool bThrusting;

    void AddTorqueToThePlane(FVector Direction, float InputVal) const;

    void Thrusting(float InputVal);

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

    UFUNCTION()
    virtual void HasDotChangedEventCaller(const float fNewDot);

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

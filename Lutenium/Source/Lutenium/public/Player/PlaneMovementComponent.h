
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
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UPlaneMovementComponent : public UActorComponent
{
    GENERATED_BODY()



protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    class APawn* OwnerPawn;

    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    class UPrimitiveComponent* PhysicsComponent;

    UPlaneMovementComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;


    // ------------------------------------------------------------------
    // Inputs
    // ------------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "Input")
    void ThrustInput(float Val);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void PitchInput(float Val);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void YawnInput(float Val);

    UFUNCTION(BlueprintCallable, Category = "Input")
    void RollInput(float Val);



    // ------------------------------------------------------------------
    // Pawn setters
    // ------------------------------------------------------------------
    FORCEINLINE UFUNCTION(Category = "General")
    void SetBox(class UPrimitiveComponent* PrimitiveComp){ PhysicsComponent = PrimitiveComp; }
    FORCEINLINE UFUNCTION(Category = "General")
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

    UPROPERTY(EditDefaultsOnly, Category = "Control")
    float LerpVelocity;

    UPROPERTY(EditDefaultsOnly, Category = "Control")
    float LerpVelocityNoThrust;

    // ------------------------------------------------------------------
    // SPEED
    // ------------------------------------------------------------------

    /* Added acceleration while thrusting */
    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float ThrustUpAcceleration;

    /* Added acceleration while stopping */
    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float ThrustDownAcceleration;

    /* Deceleration while no thrust is applied */
    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float NoThrustDeceleration;

    /* If the speed is below this value, the acceleration "kicks in" faster*/
    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float MaxAccelerationUntilTakeOff;

    /* If the speed is below this value, the acceleration "kicks in" faster*/
    UPROPERTY(Category = Speed, EditDefaultsOnly)
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
    UPROPERTY(Category = CustomPhysics, EditAnywhere)
    float CustomMaxGravity;

    UPROPERTY(Category = CustomPhysics, EditAnywhere)
    float CustomMinGravity;


    /* Aerodynamic is applied all the time */
    /* Aerodynamic vector =  Opposite velocity vector * Multiplier*/
    UPROPERTY(Category = Control, EditDefaultsOnly)
    float AerodynamicMultiplier;

    UPROPERTY(BlueprintReadOnly, Category = "Physics")
    float Dot;

    void AddGravityForce(float DeltaTime) const;

    /* Fires the event when Dot value changes drastically to enable VFX in blueprints */
    void HasDotChanged(float DotProduct);

    UFUNCTION()
    virtual void HasDotChangedEventCaller();

    void CalculateAerodynamic(float DeltaTime);


    // ------------------------------------------------------------------
    // MovementEffects
    // ------------------------------------------------------------------
    TArray<UMovementEffect*> MovementEffects;

    // ------------------------------------------------------------------
    // DEBUG
    // ------------------------------------------------------------------
    UPROPERTY(Category = Debug, EditDefaultsOnly, BlueprintReadWrite)
    bool bDeactivateThrust;

    UPROPERTY(Category = Debug, EditDefaultsOnly, BlueprintReadWrite)
    bool bDeactivateAerodynamics;

    UPROPERTY(Category = Debug, EditDefaultsOnly, BlueprintReadWrite)
    bool bDeactivateGravity;
};

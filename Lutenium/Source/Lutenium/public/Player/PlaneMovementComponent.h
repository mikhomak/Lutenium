#pragma once

#include "CoreMinimal.h"
//#include "MovementEffect/DragMovementEffect.h"
#include "MovementEffect/MovementEffect.h"
#include "Components/ActorComponent.h"
#include "PlaneMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UPlaneMovementComponent final : public UActorComponent
{
    GENERATED_BODY()



protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(BlueprintReadWrite, Category = Player, EditAnywhere, meta = (AllowPrivateAccess = "true"))
    class APlayerPawn* PlayerPawn;

    UPROPERTY(BlueprintReadWrite, Category = Mesh, EditAnywhere)
    class UBoxComponent* PlayerBox;

    UPlaneMovementComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;


    // ------------------------------------------------------------------
    // Inputs
    // ------------------------------------------------------------------
    UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
    void ThrustInput(float Val);

    UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
    void PitchInput(float Val);

    UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
    void YawnInput(float Val);

    UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
    void RollInput(float Val);

    UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
    void DashInput();

    // ------------------------------------------------------------------
    // Pawn setters
    // ------------------------------------------------------------------
    FORCEINLINE UFUNCTION(Category="General")
    void SetBox(class UBoxComponent* Mesh){ PlayerBox = Mesh; }
    FORCEINLINE UFUNCTION(Category="General")
    void SetPawn(class APlayerPawn* Pawn){ PlayerPawn = Pawn; }

    // ------------------------------------------------------------------
    // Movement
    // ------------------------------------------------------------------
    FORCEINLINE float GetCurrentAcceleration() {return CurrentAcceleration;}

    FORCEINLINE void ResetCurrentAcceleration() { CurrentAcceleration = 0;}

    UPROPERTY(Category = "Dash", EditAnywhere)
    int MaxDashes;

    UPROPERTY(Category = "Dash", EditAnywhere)
    float DashCooldown;

    UPROPERTY(Category = Control, BlueprintReadWrite, EditAnywhere)
    bool bStalling;

    UPROPERTY(Category = Speed, BlueprintReadOnly, EditDefaultsOnly)
    float MaxThrustUpAcceleration;

    UPROPERTY(Category = Speed, BlueprintReadOnly, EditDefaultsOnly)
    float MaxThrustDownAcceleration;

    void AddAcceleration(float AddedAcceleration);

    UPROPERTY(Category = Speed, BlueprintReadWrite, EditDefaultsOnly)
    float ExitStallAcceleration;


    // ------------------------------------------------------------------
    // MovementEffects
    // ------------------------------------------------------------------

    UPROPERTY(Category = Effects, BlueprintReadOnly, EditDefaultsOnly)
    class UDragMovementEffect* DragMovementEffect;

    UPROPERTY(Category = Effects, BlueprintReadOnly, EditDefaultsOnly)
    class UEmpMovementEffect* EmpMovementEffect;

protected:

    // ------------------------------------------------------------------
    // CONTROLS
    // ------------------------------------------------------------------
    UPROPERTY(Category = Control, EditDefaultsOnly)
    float AirControl;

    UPROPERTY(Category = Control, EditDefaultsOnly)
    float YawnControl;

    UPROPERTY(Category = Control, EditDefaultsOnly)
    float PitchControl;

    UPROPERTY(Category = Control, EditDefaultsOnly)
    float RollControl;

    // ------------------------------------------------------------------
    // SPEED
    // ------------------------------------------------------------------

    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float MaxSpeed;

    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float MinSpeed;

    /* Alpha to lerp the speed when the value is above MaxSpeed(could happen with dash) */
    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float MaxSpeedLerpAlpha;

    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float CurrentAcceleration;

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
    // DASH
    // ------------------------------------------------------------------
    UPROPERTY(Category = Dash, EditDefaultsOnly)
    float DashImpactForce;

    int DashesLeft;

    bool bCanDash;

    void ResetDashCooldown();


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

    float Dot;

    void AddGravityForce(float DeltaTime) const;

    /* Fires the event when Dot value changes drastically to enable VFX in blueprints */
    void HasDotChangedEventCaller(float DotProduct);

    void CalculateAerodynamic(float DeltaTime);


    // ------------------------------------------------------------------
    // MovementEffects
    // ------------------------------------------------------------------
    TArray<UMovementEffect*> MovementEffects;

};

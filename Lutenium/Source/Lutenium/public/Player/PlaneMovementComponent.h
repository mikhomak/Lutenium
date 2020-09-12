#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlaneMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UPlaneMovementComponent final : public UActorComponent
{
    GENERATED_BODY()

    UPROPERTY(Category = Player, EditAnywhere, meta = (AllowPrivateAccess = "true"))
    class APlayerPawn* PlayerPawn;

    UPROPERTY(Category = Mesh, EditAnywhere)
    class USkeletalMeshComponent* PlayerMesh;


protected:
    virtual void BeginPlay() override;

public:
    
    UPlaneMovementComponent();
    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;


    /*
     *  INPUT METHODS
     */
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

    /*
     * Player setters
     */
    UFUNCTION(BlueprintCallable, Category = "Pawn", meta = (AdvancedDisplay = "2"))
    void SetMesh(USkeletalMeshComponent* Mesh);

    UFUNCTION(BlueprintCallable, Category = "Pawn", meta = (AdvancedDisplay = "2"))
    void SetPawn(APlayerPawn* Pawn);

    /*
     * Movement 
     */
    UFUNCTION(BlueprintCallable, Category = "Control")
    float GetCurrentAcceleration() const;

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

private:

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
    // STALLING
    // ------------------------------------------------------------------
    
    /* Min speed to start stalling */
    UPROPERTY(Category = Stall, EditDefaultsOnly)
    float MinSpeedToStall;

    /* Acceleration(Not the velocity) to exit stalling */
    UPROPERTY(Category = Stall, EditDefaultsOnly)
    float AccelerationToExitStall;

    /* Time needed, while the speed is below MinSpeedToStall, to start stalling */
    UPROPERTY(Category = Stall, EditDefaultsOnly)
    float TimeToEnterStall;

    UPROPERTY(Category = CustomPhysics, EditDefaultsOnly)
    float StallForce;

    FTimerHandle StallTimer;
    
    void Stalling() const;

    void IsAboutToStall();

    void EnterStallingTimer();

    
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

    UPROPERTY(Category = Speed, EditAnywhere)
    float CurrentAcceleration;

    /* Added acceleration while thrusting */
    UPROPERTY(Category = Speed, EditAnywhere)
    float ThrustUpAcceleration;

    /* Added acceleration while stopping */
    UPROPERTY(Category = Speed, EditAnywhere)
    float ThrustDownAcceleration;

    /* Deceleration while no thrust is applied */
    UPROPERTY(Category = Speed, EditAnywhere)
    float NoThrustDeceleration;

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

    /* If the speed is below this value, the acceleration "kicks in" faster*/
    UPROPERTY(Category = Assist, EditAnywhere)
    float MaxSpeedUntilTakeOff;

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


};

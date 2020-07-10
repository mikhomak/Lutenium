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

    UFUNCTION(BlueprintCallable, Category = "Pawn", meta = (AdvancedDisplay = "2"))
    void SetMesh(USkeletalMeshComponent* Mesh);

    UFUNCTION(BlueprintCallable, Category = "Pawn", meta = (AdvancedDisplay = "2"))
    void SetPawn(APlayerPawn* Pawn);

    UFUNCTION(BlueprintCallable, Category = "Control", meta = (AdvancedDisplay = "2"))
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

    UPROPERTY(Category = Control, EditDefaultsOnly)
    float AirControl;

    UPROPERTY(Category = Control, EditDefaultsOnly)
    float YawnControl;

    UPROPERTY(Category = Control, EditDefaultsOnly)
    float PitchControl;

    UPROPERTY(Category = Control, EditDefaultsOnly)
    float RollControl;

    UPROPERTY(Category = Control, EditDefaultsOnly)
    float AerodynamicMultiplier;

    UPROPERTY(Category = Stall, EditDefaultsOnly)
    float MinSpeedToStall;

    UPROPERTY(Category = Stall, EditDefaultsOnly)
    float AccelerationToExitStall;

    UPROPERTY(Category = Stall, EditDefaultsOnly)
    float TimeToEnterStall;

    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float MaxSpeed;

    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float MinSpeed;

    UPROPERTY(Category = Speed, EditDefaultsOnly)
    float MaxSpeedLerpAlpha;

    UPROPERTY(Category = Dash, EditDefaultsOnly)
    float DashImpactForce;

    UPROPERTY(Category = Speed, EditAnywhere)
    float CurrentAcceleration;

    UPROPERTY(Category = Speed, EditAnywhere)
    float ThrustUpAcceleration;

    UPROPERTY(Category = Speed, EditAnywhere)
    float ThrustDownAcceleration;

    UPROPERTY(Category = Speed, EditAnywhere)
    float NoThrustDeceleration;

    UPROPERTY(Category = Assist, EditAnywhere)
    float MaxSpeedUntillTakeOff;

    UPROPERTY(Category = CustomPhysics, EditAnywhere)
    float CustomMaxGravity;

    UPROPERTY(Category = CustomPhysics, EditAnywhere)
    float CustomMinGravity;

    UPROPERTY(Category = CustomPhysics, EditAnywhere)
    float StallForce;

    float CurrentThrust;

    bool bThrustUp;

    bool bThrusting;

    float Dot;

    int DashesLeft;

    bool bCanDash;

    FTimerHandle StallTimer;

    void AddTorqueToThePlane(FVector Direction, float InputVal) const;

    void Thrusting(float InputVal);

    void AddThrust(float DeltaTime) const;

    void CalculateAcceleration();

    void AddGravityForce(float DeltaTime) const;
    void HasDotChangedEventCaller(float DotProduct);

    void CalculateAerodynamic(float DeltaTime);

    void ResetDashCooldown();

    void Stalling() const;

    void IsAboutToStall();

    void Movement(const float DeltaTime);

    void EnterStallingTimer();
};

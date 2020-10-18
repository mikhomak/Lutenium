#include "../public/Player/PlaneMovementComponent.h"
#include "../public/Player/PlayerPawn.h"
#include "../public/AssistUtils/AssistUtils.h"
#include "../public/Player/MovementEffect/DragMovementEffect.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "Components/SkeletalMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"

UPlaneMovementComponent::UPlaneMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostPhysics;

    MaxThrustUpAcceleration = 250000.f;
    MaxThrustDownAcceleration = 5000.f;
    MaxSpeed = 5000.f;
    MinSpeed = 50.f;
    MaxSpeedLerpAlpha = 0.4f;
    ThrustUpAcceleration = 3500.f;
    ThrustDownAcceleration = -2000.f;
    NoThrustDeceleration = -500.f;

    MaxSpeedUntilTakeOff = 500.f;

    CustomMaxGravity = -800.f;
    CustomMinGravity = -100.f;

    AirControl = 0.1f;
    YawnControl = 0.5f;
    PitchControl = 1.4f;
    RollControl = 1.f;
    AerodynamicMultiplier = 0.4f;

    Dot = 0;

    DashImpactForce = 8000.f;
    MaxDashes = 2;
    DashesLeft = MaxDashes;
    DashCooldown = 3.f;
    bCanDash = true;



}

void UPlaneMovementComponent::BeginPlay()
{
    Super::BeginPlay();
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlaneMovementComponent::CalculateAcceleration, 0.05f,
                                           true);
    DragMovementEffect = NewObject<UDragMovementEffect>();
    DragMovementEffect->InitEffect(PlayerPawn);
    MovementEffects.Init(DragMovementEffect, 1);

}

auto UPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction) -> void
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    PlayerMesh->AddTorqueInDegrees(PlayerMesh->GetPhysicsAngularVelocityInDegrees() * -1.f / 0.5f, FName(), true);
    AddGravityForce(DeltaTime);
    Movement(DeltaTime);
    for (UMovementEffect* MovementEffect : MovementEffects)
    {
        MovementEffect->ApplyEffect();
    }
}


void UPlaneMovementComponent::ThrustInput(const float Val)
{
    Thrusting(Val);
}

void UPlaneMovementComponent::PitchInput(const float Val)
{
    const float AppliedPitch = !bThrustUp ? Val * 2 : Val;
    AddTorqueToThePlane(PlayerPawn->GetActorRightVector(), AppliedPitch * PitchControl);
}

void UPlaneMovementComponent::YawnInput(const float Val)
{
    AddTorqueToThePlane(PlayerPawn->GetActorUpVector(), Val * YawnControl);
}

void UPlaneMovementComponent::RollInput(const float Val)
{
    AddTorqueToThePlane(PlayerPawn->GetActorForwardVector(), Val * RollControl);
}

void UPlaneMovementComponent::DashInput()
{
    if (DashesLeft <= 0)
    {
        return;
    }
    bCanDash = false;
    PlayerPawn->DashImpact();
    PlayerMesh->AddForce(PlayerMesh->GetForwardVector() * DashImpactForce, FName(), true);
    CurrentAcceleration = MaxThrustUpAcceleration;
    DashesLeft--;
    FTimerHandle DashCooldownTimer;
    GetWorld()->GetTimerManager().SetTimer(DashCooldownTimer, this, &UPlaneMovementComponent::ResetDashCooldown,
                                           DashCooldown,
                                           false);
}


void UPlaneMovementComponent::Movement(const float DeltaTime)
{

    CalculateAerodynamic(DeltaTime);
    AddThrust(DeltaTime);
}


void UPlaneMovementComponent::AddTorqueToThePlane(const FVector Direction, const float InputVal) const
{
    if (InputVal != 0)
    {
        const FVector ZeroVector;
        const FVector DirectionToTilt = FMath::Lerp(ZeroVector, Direction * InputVal * AirControl, 0.1f);
        PlayerMesh->AddTorqueInRadians(DirectionToTilt, FName(), true);
    }
}

void UPlaneMovementComponent::Thrusting(float InputVal)
{
    bThrusting = InputVal != 0;
    bThrustUp = InputVal > 0 ? true : false;
    if (bThrusting)
    {
        FAssistUtils::ApplyTakeOffAcceleration(this, PlayerMesh->GetPhysicsLinearVelocity(), MaxSpeedUntilTakeOff,
                                               1000.f);
    }
}


// Lerping the speed to the maximum if the current acceleration is greater than MaxSpeed(Allows dashing), and in other case clamping it to the maxSpeed
void UPlaneMovementComponent::AddThrust(float DeltaTime) const
{
    const float Speed = CurrentAcceleration > MaxSpeed
                            ? FMath::Lerp(MaxSpeed, CurrentAcceleration, MaxSpeedLerpAlpha)
                            : FMath::Clamp(CurrentAcceleration, MinSpeed, MaxSpeed);

    const FVector Velocity = FMath::Lerp(PlayerMesh->GetPhysicsLinearVelocity(), PlayerMesh->GetForwardVector() * Speed,
                                         0.014f);
    PlayerMesh->SetPhysicsLinearVelocity(Velocity, false, FName());
}

void UPlaneMovementComponent::CalculateAcceleration()
{
    CurrentAcceleration += bThrustUp
                               ? ThrustUpAcceleration
                               : (bThrusting ? ThrustDownAcceleration : NoThrustDeceleration);
    CurrentAcceleration = FMath::Clamp(CurrentAcceleration, MaxThrustDownAcceleration, MaxThrustUpAcceleration);
}

void UPlaneMovementComponent::AddGravityForce(float DeltaTime) const
{
    // The faster we travel, the less gravity is applied
    const float GravityDependingOnSpeed = FMath::GetMappedRangeValueClamped(FVector2D(MinSpeed, MaxSpeed),
                                                                            FVector2D(CustomMaxGravity,
                                                                                      CustomMinGravity),
                                                                            CurrentAcceleration);
    FVector MeshUpVectorNormalized = PlayerMesh->GetUpVector();
    MeshUpVectorNormalized.Normalize();
    const float AppliedGravity = FVector::DotProduct(MeshUpVectorNormalized, FVector(0, 0, 1)) *
        GravityDependingOnSpeed;
    PlayerMesh->AddForce(FVector(0, 0, AppliedGravity), FName(), true);
}


void UPlaneMovementComponent::CalculateAerodynamic(float DeltaTime)
{
    FVector Velocity = PlayerPawn->GetVelocity();
    const FVector UpVector = PlayerPawn->GetActorUpVector();
    Velocity.Normalize();
    const float DotProduct = FVector::DotProduct(UpVector, Velocity);
    if (DotProduct < 0)
    {
        PlayerMesh->AddForce(Velocity * DotProduct * AerodynamicMultiplier, FName(), true);
    }
    HasDotChangedEventCaller(DotProduct);
}

///		Checks if the dot value(aerodynamics) has reached the change value
///		If so, calls the DotHasChange method of PlayerPawn
///		In blueprint activates VFX when it happens
void UPlaneMovementComponent::HasDotChangedEventCaller(const float DotProduct)
{
    const float AbsPreviousDot = Dot < 0 ? Dot * -1.f : Dot;
    const float AbsDot = DotProduct < 0 ? DotProduct * -1.f : DotProduct;
    if ((AbsPreviousDot > 0.6f && AbsDot < 0.6f) || (AbsPreviousDot < 0.6f && AbsDot > 0.6f))
    {
        PlayerPawn->DotHasChange();
    }
    Dot = DotProduct;
}

void UPlaneMovementComponent::ResetDashCooldown()
{
    bCanDash = true;
    if (DashesLeft < MaxDashes)
    {
        DashesLeft++;
    }
}

void UPlaneMovementComponent::SetMesh(USkeletalMeshComponent* Mesh)
{
    PlayerMesh = Mesh;
}

void UPlaneMovementComponent::SetPawn(APlayerPawn* Pawn)
{
    PlayerPawn = Pawn;
}

float UPlaneMovementComponent::GetCurrentAcceleration() const
{
    return CurrentAcceleration;
}

void UPlaneMovementComponent::AddAcceleration(const float AddedAcceleration)
{
    CurrentAcceleration += AddedAcceleration;
}

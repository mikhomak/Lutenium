#include "Player/PlaneMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "AssistUtils/AssistUtils.h"
#include "Player/MovementEffect/DragMovementEffect.h"
#include "Player/MovementEffect/EmpMovementEffect.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "GameFramework/SpringArmComponent.h"

UPlaneMovementComponent::UPlaneMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;

    MaxThrustUpAcceleration = 25000.f;
    MaxThrustDownAcceleration = 5000.f;
    MaxSpeed = 49000.f;
    MinSpeed = 50.f;
    MaxSpeedLerpAlpha = 0.4f;
    ThrustUpAcceleration = 400.f;
    ThrustDownAcceleration = -2000.f;
    NoThrustDeceleration = -1500.f;

    LerpVelocity = 0.02f;
	LerpVelocityNoThrust = 0.03f;

    ExitStallAcceleration = 18000.f;
    MaxAccelerationUntilTakeOff = 6000.f;
    TakeOffAddedAcceleration = 12000.f;

    CustomMaxGravity = -800.f;
    CustomMinGravity = -100.f;

    AirControl = 40.f;
    YawnControl = 0.5f;
    PitchControl = 0.8f;
    RollControl = 2.8f;
    NoThrustPitchControl = 1.2f;
    AerodynamicMultiplier = 14000.f;

    Dot = 0;

    /** Movement effects */
    DragMovementEffectClass = UDragMovementEffect::StaticClass();
    EmpMovementEffectClass = UEmpMovementEffect::StaticClass();

}

void UPlaneMovementComponent::BeginPlay()
{
    Super::BeginPlay();
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlaneMovementComponent::CalculateAcceleration, 0.05f,
                                           true);
    DragMovementEffect = NewObject<UDragMovementEffect>(this, DragMovementEffectClass);
    DragMovementEffect->InitEffect(PhysicsComponent, this);
    EmpMovementEffect = NewObject<UEmpMovementEffect>(this, EmpMovementEffectClass);
    EmpMovementEffect->InitEffect(PhysicsComponent, this);
    MovementEffects.Init(DragMovementEffect, 1);
    MovementEffects.Add(EmpMovementEffect);
}

void UPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    PhysicsComponent->AddTorqueInDegrees(PhysicsComponent->GetPhysicsAngularVelocityInDegrees() * -1.f / 0.5f, FName(), true);
    AddGravityForce(DeltaTime);
    if(!bStalling)
    {
        Movement(DeltaTime);
    }
    else if(CurrentAcceleration > ExitStallAcceleration)
    {
        EmpMovementEffect->Deactivate();
        bStalling = false;
    }
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
    const float AppliedPitchControl = bThrustUp ? PitchControl : NoThrustPitchControl;
    AddTorqueToThePlane(OwnerPawn->GetActorRightVector(), AppliedPitchControl * Val);
}

void UPlaneMovementComponent::YawnInput(const float Val)
{
    AddTorqueToThePlane(OwnerPawn->GetActorUpVector(), Val * YawnControl);
}

void UPlaneMovementComponent::RollInput(const float Val)
{
    AddTorqueToThePlane(OwnerPawn->GetActorForwardVector(), Val * RollControl);
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
        const FVector DirectionToTilt = FMath::Lerp(FVector::ZeroVector, Direction * InputVal * AirControl, 0.1f);
        PhysicsComponent->AddTorqueInRadians(DirectionToTilt, FName(), true);
    }
}

void UPlaneMovementComponent::Thrusting(float InputVal)
{
    bThrusting = InputVal != 0;
    bThrustUp = InputVal > 0 ? true : false;
    if (bThrustUp && CurrentAcceleration < MaxAccelerationUntilTakeOff  && !bHasAppliedTakeOffAcceleration)
    {
        CurrentAcceleration += TakeOffAddedAcceleration;
        bHasAppliedTakeOffAcceleration = true;
    }
    else if(CurrentAcceleration > MaxAccelerationUntilTakeOff)
    {
        bHasAppliedTakeOffAcceleration = false;
    }
}


// Lerping the speed to the maximum if the current acceleration is greater than MaxSpeed(Allows dashing), and in other case clamping it to the maxSpeed
void UPlaneMovementComponent::AddThrust(float DeltaTime) const
{
    if(bDeactivateThrust)
    {
        return;
    }
    const float Speed = CurrentAcceleration > MaxSpeed
                            ? FMath::Lerp(MaxSpeed, CurrentAcceleration, MaxSpeedLerpAlpha)
                            : FMath::Clamp(CurrentAcceleration, MinSpeed, MaxSpeed);

    const FVector Velocity = FMath::Lerp(PhysicsComponent->GetPhysicsLinearVelocity(), PhysicsComponent->GetForwardVector() * Speed,
                                        bThrustUp ? LerpVelocity : LerpVelocityNoThrust);
    PhysicsComponent->SetPhysicsLinearVelocity(Velocity, false, FName());
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
    if(bDeactivateGravity)
    {
        return;
    }
    // The faster we travel, the less gravity is applied
    const float GravityDependingOnSpeed = FMath::GetMappedRangeValueClamped(FVector2D(MinSpeed, MaxSpeed),
                                                                            FVector2D(CustomMaxGravity,
                                                                                      CustomMinGravity),
                                                                            CurrentAcceleration);
    FVector UpVectorNormalized = PhysicsComponent->GetUpVector();
    UpVectorNormalized.Normalize();
    const float AppliedGravity = FVector::DotProduct(UpVectorNormalized, FVector(0, 0, 1)) *
        GravityDependingOnSpeed;
    PhysicsComponent->AddForce(FVector(0, 0, AppliedGravity), FName(), true);
}


void UPlaneMovementComponent::CalculateAerodynamic(float DeltaTime)
{
    if(bDeactivateAerodynamics)
    {
        return;
    }
    FVector Velocity = OwnerPawn->GetVelocity();
    const FVector UpVector = OwnerPawn->GetActorUpVector();
    Velocity.Normalize();
    const float DotProduct = FVector::DotProduct(UpVector, Velocity);


    if (DotProduct < 0)
    {
        // The greater our speed, the greatere the aerodynamic effect
        const FVector AppliedAerodynamic = Velocity * DotProduct * AerodynamicMultiplier *
        (
            (CurrentAcceleration - MinSpeed)
                        /
            (MaxSpeed - MinSpeed)
        ); // Maping current velocity value between 0 and 1

        PhysicsComponent->AddForce(AppliedAerodynamic, FName(), true);
    }
    HasDotChanged(DotProduct);
}

///		Checks if the dot value(aerodynamics) has reached the change value
///		If so, calls the DotHasChange method of OwnerPawn
///		In blueprint activates VFX when it happens
void UPlaneMovementComponent::HasDotChanged(const float DotProduct)
{
    const float AbsPreviousDot = Dot < 0 ? Dot * -1.f : Dot;
    const float AbsDot = DotProduct < 0 ? DotProduct * -1.f : DotProduct;
    if ((AbsPreviousDot > 0.6f && AbsDot < 0.6f) || (AbsPreviousDot < 0.6f && AbsDot > 0.6f))
    {
        HasDotChangedEventCaller();
    }
    Dot = DotProduct;
}


void UPlaneMovementComponent::HasDotChangedEventCaller()
{

}

void UPlaneMovementComponent::AddAcceleration(const float AddedAcceleration)
{
    CurrentAcceleration += AddedAcceleration;
}

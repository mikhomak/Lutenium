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
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPlaneMovementComponent::UPlaneMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;

    MinThrustAcceleration = 5000.f;
    MaxAcceleration = 25000.f;
    MinAcceleration = 50.f;
    MaxSpeedLerpAlpha = 0.4f;
    ThrustUpAcceleration = 400.f;
    ThrustDownAcceleration = -2000.f;
    NoThrustDeceleration = -1500.f;

    LerpVelocity = 0.02f;
	LerpVelocityNoThrust = 0.03f;

    ExitStallAcceleration = 18000.f;

    // Kick in
    MinAccelerationUntilKickIn = 8000.f;
    MaxAccelerationUntilKickIn = 15000.f;
    KickInAddedAcceleration = 12000.f;
    CurrentKickInAcceleration = 0.f;

    CustomMaxGravity = -800.f;
    CustomMinGravity = -100.f;

    AirControl = 40.f;
    YawnControl = 0.5f;
    PitchControl = 0.8f;
    RollControl = 2.8f;
    NoThrustPitchControl = 1.2f;
    AerodynamicMultiplier = 28000.f;

    Dot = 0;
    DotThreshold = 0.6f;

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
    fDeltaTime = DeltaTime;
    PhysicsComponent->AddTorqueInDegrees(PhysicsComponent->GetPhysicsAngularVelocityInDegrees() * -1.f / 0.5f, FName(), true);
    AddGravityForce(DeltaTime);
    if(!bStalling)
    {
        Movement(DeltaTime);
    }
    for (UMovementEffect* MovementEffect : MovementEffects)
    {
        MovementEffect->ApplyEffect();
    }
}


void UPlaneMovementComponent::ThrustInput(const FInputActionValue& Value)
{
    Thrusting(Value.GetMagnitude());
}

void UPlaneMovementComponent::PitchInput(const FInputActionValue& Value)
{
    const float fAppliedPitchControl = bThrustUp ? PitchControl : NoThrustPitchControl;
    AddTorqueToThePlane(OwnerPawn->GetActorRightVector(), fAppliedPitchControl * Value.GetMagnitude());
}

void UPlaneMovementComponent::YawnInput(const FInputActionValue& Value)
{
    AddTorqueToThePlane(OwnerPawn->GetActorUpVector(), Value.GetMagnitude() * YawnControl);
}

void UPlaneMovementComponent::RollInput(const FInputActionValue& Value)
{
    AddTorqueToThePlane(OwnerPawn->GetActorForwardVector(), Value.GetMagnitude() * RollControl);
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
    if (bThrustUp && CurrentAcceleration < MaxAccelerationUntilKickIn && CurrentAcceleration > MinAccelerationUntilKickIn  && !bHasAppliedKickInAcceleration)
    {
        CurrentKickInAcceleration = KickInAddedAcceleration;
        CurrentAcceleration += KickInAddedAcceleration;
        bHasAppliedKickInAcceleration = true;
        OnKickinAcceleration.Broadcast();
    }
    else if(CurrentAcceleration > MinAccelerationUntilKickIn)
    {
        bHasAppliedKickInAcceleration = false;
    }

    if(bStalling && CurrentAcceleration > ExitStallAcceleration)
    {
        EmpMovementEffect->Deactivate();
        bStalling = false;
    }
}


// Lerping the speed to the maximum if the current acceleration is greater than MaxAcceleration(Allows dashing), and in other case clamping it to the MaxAcceleration
void UPlaneMovementComponent::AddThrust(float DeltaTime)
{
    if(bDeactivateThrust)
    {
        return;
    }
    // If the acceleration somehow is beyond the MaxAcceleration,
    // We don't force clamp it, but rather lerping it into MaxAcceleration
    float Speed = CurrentAcceleration > MaxAcceleration
                            ? FMath::Lerp(MaxAcceleration, CurrentAcceleration, MaxSpeedLerpAlpha)
                            : CurrentAcceleration;
    
    if(CurrentKickInAcceleration > 0.f)
    {
    	Speed += CurrentKickInAcceleration;
    	CurrentKickInAcceleration = 0.f;
    }                            	

    const FVector Velocity = FMath::Lerp(PhysicsComponent->GetPhysicsLinearVelocity(), PhysicsComponent->GetForwardVector() * Speed,
                                        bThrustUp ? LerpVelocity : LerpVelocityNoThrust);
    PhysicsComponent->SetPhysicsLinearVelocity(Velocity, false, FName());
}

void UPlaneMovementComponent::CalculateAcceleration()
{
    CurrentAcceleration += bThrustUp
                               ? ThrustUpAcceleration
                               : (bThrusting ? ThrustDownAcceleration : NoThrustDeceleration);
    CurrentAcceleration = FMath::Clamp(CurrentAcceleration, MinAcceleration, MaxAcceleration);
}

void UPlaneMovementComponent::AddGravityForce(float DeltaTime) const
{
    if(bDeactivateGravity)
    {
        return;
    }
    // The faster we travel, the less gravity is applied
    const float GravityDependingOnSpeed = FMath::GetMappedRangeValueClamped(FVector2D(MinAcceleration, MaxAcceleration),
                                                                            FVector2D(CustomMaxGravity,
                                                                                      CustomMinGravity),
                                                                            CurrentAcceleration);
    FVector UpVectorNormalized = PhysicsComponent->GetUpVector();
    UpVectorNormalized.Normalize();
    const float AppliedGravity = UKismetMathLibrary::Abs(FVector::DotProduct(UpVectorNormalized, FVector(0, 0, 1))) *
        GravityDependingOnSpeed;
    // GravityDependingOnSpeed is negative, so the vector will point
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
    const float fAbsDotProduct = UKismetMathLibrary::Abs(DotProduct);

    if (bApplyAerodynamicsOnSpecificValue_DEBUG || fAbsDotProduct > DotThreshold)
    {
        // The greater our speed, the greatere the aerodynamic effect
        const float fSpeedLerp =
         (!bScaleAerodynamicsWithSpeed_DEBUG ?
            (
                (CurrentAcceleration - MinAcceleration)
                            /
                (MaxAcceleration - MinAcceleration)
            ) // Maping current velocity value between 0 and 1
            :    1);
        const FVector AppliedAerodynamic = Velocity * fAbsDotProduct * AerodynamicMultiplier * fSpeedLerp * -1.f;

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
    if ((AbsPreviousDot > DotThreshold && AbsDot < DotThreshold) || (AbsPreviousDot < DotThreshold && AbsDot > DotThreshold))
    {
        OnDotHasChanged.Broadcast(DotProduct);
    }
    Dot = DotProduct;
}


void UPlaneMovementComponent::AddAcceleration(const float AddedAcceleration)
{
    CurrentAcceleration += AddedAcceleration;
}

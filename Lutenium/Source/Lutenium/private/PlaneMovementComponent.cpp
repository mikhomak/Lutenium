#include "../public/PlaneMovementComponent.h"
#include "../public/PlayerPawn.h"
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

	MaxThrustUpAcceleration = 3000.f;
	ThrustMaxSpeed = 8000.f;
	ThrustMinSpeed = 50.f;

	NoThrustDeceleration = -3.f;
	ThrustUpAcceleration = 1.f;
	ThrustDownAcceleration = 10.f;

	CustomMaxGravity = -800.f;
	CustomMinGravity = -100.f;


	AirControl = 0.1f;
	YawnControl = 0.5f;
	PitchControl = 1.4f;
	RollControl = 1.f;
	AerodynamicMultiplier = 0.4f;

	Dot = 0;
	DashImpactForce = 8000;
	MaxDashes = 3;
	DashesLeft = MaxDashes;
	DashCooldown = 1.f;
	bCanDash = true;

	MaxSpeedLerpAlpha = 0.2f;


	MinSpeedToStall = 200.f;
	StallForce = -1000.f;
	TimeToEnterStall = 1.1f;
	bStalling = false;
}


void UPlaneMovementComponent::BeginPlay()
{
	MaxThrustDownAcceleration = MaxThrustUpAcceleration * -1.f;
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlaneMovementComponent::CalculateAcceleration, 0.05f,
	                                       true);
}

auto UPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction) -> void
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PlayerMesh->AddTorqueInDegrees(PlayerMesh->GetPhysicsAngularVelocityInDegrees() * -1.f / 0.5f, FName(), true);
	AddGravityForce(DeltaTime);
	IsAboutToStall();
	Movement(DeltaTime);
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
	if (bStalling == false)
	{
		CalculateAerodynamic(DeltaTime);
		AddThrust(DeltaTime);
	}
	else
	{
		Stalling();
	}
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
}


// Lerping the speed to the maximum if the current acceleration is greater than MaxSpeed(Allows dashing), and in other case clamping it to the maxSpeed
void UPlaneMovementComponent::AddThrust(float DeltaTime) const
{
	const float Speed = CurrentAcceleration > ThrustMaxSpeed
		                    ? FMath::Lerp(ThrustMaxSpeed, CurrentAcceleration, MaxSpeedLerpAlpha)
		                    : FMath::Clamp(CurrentAcceleration, ThrustMinSpeed, ThrustMaxSpeed);

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
	const float GravityDependingOnSpeed = FMath::GetMappedRangeValueClamped(FVector2D(ThrustMinSpeed, ThrustMaxSpeed),
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

void UPlaneMovementComponent::IsAboutToStall()
{
	if (bStalling)
	{
		if ((MinSpeedToStall >= PlayerMesh->GetPhysicsLinearVelocity().Size() && AccelerationToExitStall <=
			CurrentAcceleration) || MinSpeedToStall <= PlayerMesh->GetPhysicsLinearVelocity().Size())
		{
			bStalling = false;
		}
	}
	if (MinSpeedToStall >= PlayerMesh->GetPhysicsLinearVelocity().Size() && !bStalling)
	{
		bStalling = true;
		GetWorld()->GetTimerManager().SetTimer(StallTimer, this, &UPlaneMovementComponent::EnterStallingTimer,
		                                       TimeToEnterStall);
	}
}

void UPlaneMovementComponent::EnterStallingTimer()
{
	if (MinSpeedToStall >= PlayerMesh->GetPhysicsLinearVelocity().Size() && AccelerationToExitStall >=
		CurrentAcceleration)
	{
		bStalling = true;
	}
	else if (MinSpeedToStall >= PlayerMesh->GetPhysicsLinearVelocity().Size() && AccelerationToExitStall <=
		CurrentAcceleration)
	{
		bStalling = false;
		GetWorld()->GetTimerManager().ClearTimer(StallTimer);
	}
}


void UPlaneMovementComponent::Stalling() const
{
	PlayerMesh->AddForce(FVector(0, 0, StallForce), FName(), true);
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

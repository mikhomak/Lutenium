#include "../public/PlaneMovementComponent.h"
#include "../public/PlayerPawn.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Math/Vector.h"
#include "Components/SkeletalMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

UPlaneMovementComponent::UPlaneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PostPhysics;

	MaxThrustUpAcceleration = 3000.f;
	ThrustMaxSpeed = 8000.f;
	ThrustMinSpeed = 50.f;
	AirControl = 0.1f;
	CustomGravity = -600.f;
	YawnControl = 0.5f;
	PitchControl = 1.f;
	RollControl = 1.f;
	AerodynamicMultiplier = 0.4f;
	DashImpact = 8000;
	Dot = 0;
	ThrustUpAcceleration = 1.f;
	ThrustDownAcceleration = 10.f;
	
}


void UPlaneMovementComponent::BeginPlay()
{
	MaxThrustDownAcceleration = MaxThrustUpAcceleration * -1.f;
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlaneMovementComponent::CalculateAcceleration, 0.05f,
	                                       true);
}

auto UPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction) -> void
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PlayerMesh->AddTorqueInDegrees(PlayerMesh->GetPhysicsAngularVelocityInDegrees() * -1.f / 0.5f, FName(), true);
	AddGravityForce(DeltaTime);
	CalculateAerodynamic(DeltaTime);
	AddThrust(DeltaTime);
}


void UPlaneMovementComponent::ThrustInput(const float Val)
{
	Thrusting(Val);
}

void UPlaneMovementComponent::PitchInput(const float Val)
{
	AddTorqueToThePlane(PlayerPawn->GetActorRightVector(), Val * PitchControl);
}

void UPlaneMovementComponent::YawnInput(const float Val)
{
	AddTorqueToThePlane(PlayerPawn->GetActorUpVector(), Val * YawnControl);
}

void UPlaneMovementComponent::RollInput(const float Val)
{
	AddTorqueToThePlane(PlayerPawn->GetActorForwardVector(), Val * RollControl);
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
	if (bThrusting)
	{
		ThrustUp = InputVal > 0 ? true : false;
	}
}

void UPlaneMovementComponent::AddThrust(float DeltaTime) const
{
	const float Speed = FMath::Clamp(CurrentAcceleration, ThrustMinSpeed, ThrustMaxSpeed);
	const FVector Velocity = FMath::Lerp(PlayerMesh->GetPhysicsLinearVelocity(), PlayerMesh->GetForwardVector() * Speed,
	                                     0.014f);
	PlayerMesh->SetPhysicsLinearVelocity(Velocity, false, FName());
}

void UPlaneMovementComponent::CalculateAcceleration()
{
	if (bThrusting)
	{
		CurrentAcceleration += ThrustUp ? ThrustUpAcceleration : ThrustDownAcceleration;
		CurrentAcceleration = FMath::Clamp(CurrentAcceleration, MaxThrustDownAcceleration, MaxThrustUpAcceleration);
	}
}

void UPlaneMovementComponent::AddGravityForce(float DeltaTime) const
{
	PlayerMesh->AddForce(FVector(0, 0, CustomGravity), FName(), true);
}

void UPlaneMovementComponent::SetMesh(USkeletalMeshComponent* Mesh)
{
	PlayerMesh = Mesh;
}

void UPlaneMovementComponent::SetPawn(APlayerPawn* Pawn)
{
	PlayerPawn = Pawn;
}

void UPlaneMovementComponent::StopInput()
{
	//float Speed = FMath::Clamp(ThrustAcceleration * Acceleration, ThrustMinSpeed, ThrustMaxSpeed);
	//FVector Velocity = PlayerPawn->GetVelocity() + PlayerMesh->GetForwardVector() * (Speed / 5);
	//PlayerMesh->SetPhysicsLinearVelocity(PlayerMesh->GetForwardVector() * (Speed / 5), true, FName());
	//PlayerPawn->LaunchPawn(PlayerMesh->GetForwardVector() * DashImpact, true, true);
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
	Dot = DotProduct;
}

float UPlaneMovementComponent::GetDot() const
{
	return Dot;
}

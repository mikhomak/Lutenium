// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneMovementComponent.h"
#include "PlayerPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Math/Vector.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"

UPlaneMovementComponent::UPlaneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PlayerPawn = (APlayerPawn*) GetOwner();

	ThrustAcceleration = 100.f;
	ThrustMaxSpeed = 8000.f;
	ThrustMinSpeed = 50.f;
	AirControl = 40.f;
	CustomGravity = -600.f;

}


void UPlaneMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerMesh = PlayerPawn->GetPlaneMesh();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlaneMovementComponent::CalculateAcceleration, 0.05f, true);
}

void UPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PlayerMesh->AddTorqueInDegrees(PlayerMesh->GetPhysicsAngularVelocityInDegrees() * -1.f / 0.5f, FName(), true);
	AddThrust();
	AddGravityForce();
}


void UPlaneMovementComponent::ThrustInput(float Val) {
	Thrusting(Val);
}

void UPlaneMovementComponent::PitchInput(float Val) {
	AddTorqueToThePlane(PlayerPawn->GetActorRightVector(), Val);
}

void UPlaneMovementComponent::YawnInput(float Val) {
	AddTorqueToThePlane(PlayerPawn->GetActorUpVector(), Val);
}

void UPlaneMovementComponent::RollInput(float Val) {
	AddTorqueToThePlane(PlayerPawn->GetActorForwardVector(), Val);
}

void UPlaneMovementComponent::AddTorqueToThePlane(FVector Direction, float InputVal) {
	FVector ZeroVector;
	FVector DirectionToTilt = FMath::Lerp(ZeroVector, Direction * InputVal * AirControl, 0.1f);
	PlayerMesh->AddTorque(DirectionToTilt, FName(), true);
}

void  UPlaneMovementComponent::Thrusting(float InputVal) {
	ThrustUp = InputVal == 0 ? false : InputVal > 0 ? true : false;
}

void UPlaneMovementComponent::AddThrust() {
	float Speed = FMath::Clamp(ThrustAcceleration * Acceleration, ThrustMinSpeed, ThrustMaxSpeed);
	FVector Velocity = FMath::Lerp(PlayerMesh->GetPhysicsLinearVelocity(), PlayerMesh->GetForwardVector() * Speed, 0.01f);
	PlayerMesh->SetPhysicsLinearVelocity(Velocity, false, FName());
}

void UPlaneMovementComponent::CalculateAcceleration() {
	Acceleration += ThrustUp ? 1 : -1;
	Acceleration = FMath::Clamp(Acceleration, 0.f, 100.f);
}

void UPlaneMovementComponent::AddGravityForce() {
	PlayerMesh->AddForce(FVector(0,0, CustomGravity), FName(), true);
}
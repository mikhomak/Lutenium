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

UPlaneMovementComponent::UPlaneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PlayerPawn = (APlayerPawn*) GetOwner();

	ThrustAcceleration = 1500.f;
	ThrustMaxSpeed = 20000.f;
	ThrustMinSpeed = 50.f;

	AirControl = 50.f;

}


void UPlaneMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerMesh = PlayerPawn->GetPlaneMesh();
}


// Called every frame
void UPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PlayerMesh->AddTorqueInDegrees(PlayerMesh->GetPhysicsAngularVelocityInDegrees() * -1.f / 0.5f, FName(), true);
	AddThrust();
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
	ThrustUp = InputVal == 0 ? false : InputVal < 0 ? true : false;
}

void UPlaneMovementComponent::AddThrust() {
	float Thrust = ThrustUp ? 1 : ThrustDown ? -1 : 0;
	float Speed = FMath::Clamp(Thrust * ThrustAcceleration, ThrustMinSpeed, ThrustMaxSpeed);
	FVector Velocity = FMath::Lerp(PlayerMesh->GetPhysicsLinearVelocity(), PlayerMesh->GetForwardVector() * Speed, 0.01f);
	PlayerMesh->SetPhysicsLinearVelocity(Velocity, false, FName());
}
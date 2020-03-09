// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaneMovementComponent.h"
#include "PlayerPawn.h"
#include "PlayerPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "GenericPlatform/GenericPlatformMath.h"

// Sets default values for this component's properties
UPlaneMovementComponent::UPlaneMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	PlayerPawn = (APlayerPawn*) GetOwner();

	// Set handling parameters
	ThrustAcceleration = 500.f;
	ThrustMaxSpeed = 2000.f;
	ThrustMinSpeed = 50.f;
	YawnSpeed = 50.f;
	RollSpeed = 100.f;
	CurrentForwardSpeed = 500.f;
	PitchSpeed = 80.f;

}


// Called when the game starts
void UPlaneMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const FVector LocalMove = FVector(CurrentForwardSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f);

	// Move plan forwards (with sweep so we stop when we collide with things)
	PlayerPawn->AddActorLocalOffset(LocalMove, true);

	// Calculate change in rotation this frame
	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * GetWorld()->GetDeltaSeconds();
	DeltaRotation.Yaw = CurrentYawSpeed * GetWorld()->GetDeltaSeconds();
	DeltaRotation.Roll = CurrentRollSpeed * GetWorld()->GetDeltaSeconds();

	// Rotate plane
	PlayerPawn->AddActorLocalRotation(DeltaRotation);

}


void UPlaneMovementComponent::ThrustInput(float Val) {
	// Is there any input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * ThrustAcceleration) : (-0.5f * ThrustAcceleration);
	// Calculate new speed
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	// Clamp between MinSpeed and MaxSpeed
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, ThrustMinSpeed, ThrustMaxSpeed);
	// If the plane is not accelarating 
	if (Val > 0.4f) {
		PlayerPawn->GetSpringArm()->bInheritPitch = false;
		//SpringArm->bUsePawnControlRotation = false;
	}
	else {
		PlayerPawn->GetSpringArm()->bInheritPitch = true;
		//SpringArm->bUsePawnControlRotation = true;
	}
}

void UPlaneMovementComponent::PitchInput(float Val) {
	// Target pitch speed is based in input
	float TargetPitchSpeed = (Val * PitchSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	// Smoothly interpolate to target pitch speed
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);



}

void UPlaneMovementComponent::YawnInput(float Val) {
	// Target yaw speed is based on input
	float TargetYawSpeed = (Val * YawnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	//const bool bIsTurning = FMath::Abs(Val) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value.

}

void UPlaneMovementComponent::RollInput(float Val) {

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, Val * RollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}
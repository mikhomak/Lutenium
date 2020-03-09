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

UPlaneMovementComponent::UPlaneMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PlayerPawn = (APlayerPawn*) GetOwner();

	ThrustAcceleration = 500.f;
	ThrustMaxSpeed = 2000.f;
	ThrustMinSpeed = 50.f;
	YawnSpeed = 50.f;
	RollSpeed = 100.f;
	CurrentForwardSpeed = 500.f;
	PitchSpeed = 80.f;

}


void UPlaneMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	const FVector LocalMove = FVector(CurrentForwardSpeed * GetWorld()->GetDeltaSeconds(), 0.f, 0.f);

	PlayerPawn->AddActorLocalOffset(LocalMove, true);

	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * GetWorld()->GetDeltaSeconds();
	DeltaRotation.Yaw = CurrentYawSpeed * GetWorld()->GetDeltaSeconds();
	DeltaRotation.Roll = CurrentRollSpeed * GetWorld()->GetDeltaSeconds();

	PlayerPawn->AddActorLocalRotation(DeltaRotation);

}


void UPlaneMovementComponent::ThrustInput(float Val) {
	// Is there any input?
	bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
	// If input is not held down, reduce speed
	float CurrentAcc = bHasInput ? (Val * ThrustAcceleration) : (-0.5f * ThrustAcceleration);
	
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, ThrustMinSpeed, ThrustMaxSpeed);
	
	// If the plane is not accelarating 
	if (Val > 0.4f) {
		PlayerPawn->GetSpringArm()->bInheritPitch = true;
		//SpringArm->bUsePawnControlRotation = false;
	}
	else {
		PlayerPawn->GetSpringArm()->bInheritPitch = false;
		//SpringArm->bUsePawnControlRotation = true;
	}
}

void UPlaneMovementComponent::PitchInput(float Val) {
	float TargetPitchSpeed = (Val * PitchSpeed * -1.f);

	// When steering, we decrease pitch slightly
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);

	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);



}

void UPlaneMovementComponent::YawnInput(float Val) {
	float TargetYawSpeed = (Val * YawnSpeed);

	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);


}

void UPlaneMovementComponent::RollInput(float Val) {

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, Val * RollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}
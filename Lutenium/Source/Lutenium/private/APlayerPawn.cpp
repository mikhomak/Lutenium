// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "../public/PlayerPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/StaticMesh.h"
#include "../public/PlaneMovementComponent.h"
#include "Components/PrimitiveComponent.h"

APlayerPawn::APlayerPawn()
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<USkeletalMesh> PlaneMesh;

		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Art/Models/airplane/airplaneBones.airplaneBones"))
		{
		}
	};

	static FConstructorStatics ConstructorStatics;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PostPhysics;

	PlaneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlaneMesh0"));
	PlaneMesh->SetSkeletalMesh(ConstructorStatics.PlaneMesh.Get());
	PlaneMesh->SetSimulatePhysics(true);
	PlaneMesh->SetEnableGravity(false);
	PlaneMesh->SetTickGroup(TG_PostUpdateWork);
	PlaneMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	RootComponent = PlaneMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm0"));
	SpringArm->SetupAttachment(RootComponent); // Attach SpringArm to RootComponent
	SpringArm->TargetArmLength = 700.0f; // The camera follows at this distance behind the character	
	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = true; // Do not allow camera to lag
	SpringArm->CameraLagSpeed = 20.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera
	Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

	PlaneMovement = CreateDefaultSubobject<UPlaneMovementComponent>(TEXT("Plane Movement"));
	PlaneMovement->SetMesh(PlaneMesh);
	PlaneMovement->SetPawn(this);
}

void APlayerPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerPawn::NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other,
                            class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation,
                            FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	const FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}


void APlayerPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	InputComponent = PlayerInputComponent;
	PlayerInputComponent->BindAxis("Thrust", PlaneMovement, &UPlaneMovementComponent::ThrustInput);
	PlayerInputComponent->BindAxis("Pitch", PlaneMovement, &UPlaneMovementComponent::PitchInput);
	PlayerInputComponent->BindAxis("Yawn", PlaneMovement, &UPlaneMovementComponent::YawnInput);
	PlayerInputComponent->BindAxis("Roll", PlaneMovement, &UPlaneMovementComponent::RollInput);
	PlayerInputComponent->BindAction("Stop", IE_Released, PlaneMovement, &UPlaneMovementComponent::DashInput);
}

float APlayerPawn::GetYawnInput() const
{
	if (InputComponent)
	{
		check(InputComponent);
		return InputComponent->GetAxisValue("Yawn");
	}
	return 0.f;
}

float APlayerPawn::GetRollInput() const
{
	if (InputComponent)
	{
		check(InputComponent);
		return InputComponent->GetAxisValue("Roll");
	}
	return 0.f;
}

float APlayerPawn::GetPitchInput() const
{
	if (InputComponent)
	{
		check(InputComponent);
		return InputComponent->GetAxisValue("Pitch");
	}
	return 0.f;
}

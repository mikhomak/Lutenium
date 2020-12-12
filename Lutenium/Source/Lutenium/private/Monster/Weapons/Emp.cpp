// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Weapons/Emp.h"
#include "Player/PlayerPawn.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/MovementEffect/EmpMovementEffect.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"


AEmp::AEmp()
{
	PrimaryActorTick.bCanEverTick = true;
	MainSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Main Sphere"));
    RootComponent = MainSphereComp;

	EmpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("First Wave"));
    EmpMesh->AttachToComponent(MainSphereComp, FAttachmentTransformRules::KeepWorldTransform);
    EmpMesh->SetGenerateOverlapEvents(true);

	EmpRotationForce = 80.f;

    ScalingMultiplier = FVector(1.f, 1.f, 0.f);
    ScalingSpeed = 0.1f;

	EmpLifeTime=15.f;
}

void AEmp::BeginPlay()
{
	Super::BeginPlay();
    /* Add overlap function calls in blueprints! */
    SetLifeSpan(EmpLifeTime);

}

void AEmp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    const FVector EmpCurrentScale = EmpMesh->GetComponentScale();
    const FVector EmpNewScale = EmpCurrentScale + ScalingMultiplier * ScalingSpeed;
    EmpMesh->SetWorldScale3D(EmpNewScale);
}

void AEmp::EmpOverlap(AActor* OtherActor)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
		const FVector RandomRotation = FVector(
			FMath::RandRange(0.f, 1.f),
			FMath::RandRange(0.f, 1.f),
			FMath::RandRange(0.f, 1.f)
		);
        PlayerPawn->GetPlaneMovement()->EmpMovementEffect->Activate(0.f, RandomRotation, EmpRotationForce);
    }
}
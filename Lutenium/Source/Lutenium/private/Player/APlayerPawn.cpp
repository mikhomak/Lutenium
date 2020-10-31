// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "../public/Player/PlayerPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../public/Player/PlaneMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "../public/Player/Missile.h"
#include "AssistUtils/AssistUtils.h"

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

    PlaneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Plane Box"));
    PlaneBox->SetSimulatePhysics(true);
    PlaneBox->SetEnableGravity(false);
    PlaneBox->SetTickGroup(TG_PostUpdateWork);
    PlaneBox->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    RootComponent = PlaneBox;

    PlaneMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Plane Mesh"));
    PlaneMesh->SetSkeletalMesh(ConstructorStatics.PlaneMesh.Get());
    PlaneMesh->SetTickGroup(TG_PostUpdateWork);
    PlaneMesh->AttachToComponent(PlaneBox, FAttachmentTransformRules::KeepWorldTransform);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent); // Attach SpringArm to RootComponent
    SpringArm->TargetArmLength = 700.0f; // The camera follows at this distance behind the character
    SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
    SpringArm->bEnableCameraLag = true; // Allow camera to lag
    SpringArm->CameraLagSpeed = 20.f;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera
    Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

    PlaneMovement = CreateDefaultSubobject<UPlaneMovementComponent>(TEXT("Plane Movement"));
    PlaneMovement->SetBox(PlaneBox);
    PlaneMovement->SetPawn(this);


    MissileTraceLength = 50000.f;
    FirstRaytraceRadius = 20000.f;
    SecondRaytraceRadius = 40000.f;
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
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerPawn::FireMissile);
}

void APlayerPawn::FireMissile()
{
    if (MissileClass)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = this;

            FVector SpawnLocation = PlaneMesh->GetSocketLocation("MissileMuzzle");

            AMissile* Missile = World->SpawnActor<AMissile>(MissileClass, SpawnLocation, GetActorRotation(),
                                                            SpawnParams);
            if (Missile)
            {
                Missile->SetParentPawn(this);


                FVector MissileDirection;
                USceneComponent* Target = FAssistUtils::RaycastMissileTarget(this,GetWorld(),
                                                                             SpawnLocation, GetActorForwardVector(),
                                                                             MissileTraceLength, FirstRaytraceRadius,
                                                                             SecondRaytraceRadius, MissileDirection);
                MissileDirection.Normalize();
                Missile->SetTargetOrDirection(Target, GetActorForwardVector());
            }
        }
    }
}

float APlayerPawn::GetThrustInput() const
{
    if (InputComponent)
    {
        check(InputComponent);
        return InputComponent->GetAxisValue("Thrust");
    }
    return 0.f;
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

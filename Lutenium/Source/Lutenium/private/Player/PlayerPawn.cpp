// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Player/PlayerPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/PlaneMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Player/Missile.h"
#include "Player/MissileTargetHit.h"
#include "AssistUtils/AssistUtils.h"

APlayerPawn::APlayerPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    PlaneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Plane Box"));
    PlaneBox->SetSimulatePhysics(true);
    PlaneBox->SetEnableGravity(false);
    PlaneBox->SetCollisionProfileName(TEXT("Player"));
    RootComponent = PlaneBox;

    const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
        EAttachmentRule::KeepRelative, false);
    PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane Mesh"));
    PlaneMesh->AttachToComponent(PlaneBox, AttachmentTransformRules);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent); // Attach SpringArm to RootComponent
    SpringArm->TargetArmLength = 700.0f; // The camera follows at this distance behind the character
    SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);


    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName); // Attach the camera
    Camera->bUsePawnControlRotation = false; // Don't rotate camera with controller

    PlaneMovement = CreateDefaultSubobject<UPlaneMovementComponent>(TEXT("Plane Movement"));
    PlaneMovement->SetPawn(this);
    PlaneMovement->SetMesh(PlaneMesh);
    PlaneMovement->SetBox(PlaneBox);


    MissileTraceLength = 50000.f;
    FirstRaytraceRadius = 20000.f;
    SecondRaytraceRadius = 40000.f;
}

void APlayerPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    /* Raycasting every tick to update MissileTargetRaycastHitLocation so we can show the UI of the current missile target(even tho it has been shot yet) */


    // If previos found location was a hurtbox, we wanna cache it
    if(MissileTargetRaycastHitType == EMissileTargetHit::MonsterWPHurtbox)
    {
        // however we need to make sure that we still can see it
        // if we don't, then look for a new one
        if(!FAssistUtils::RaycastSameMonsterPosition(this, GetWorld(), PlaneMesh->GetSocketLocation("MissileMuzzle"), MissileTargetRaycastHitLocation))
        {
            FAssistUtils::RaycastMissileTarget(this,GetWorld(),
                                                    PlaneMesh->GetSocketLocation("MissileMuzzle"), GetActorForwardVector(),
                                                    MissileTraceLength, FirstRaytraceRadius,
                                                    SecondRaytraceRadius, MissileTargetRaycastHitLocation,
                                                    MissileTargetRaycastHitType);
        }
    }
    else // If the previous found location was not hurtbox, then we don't care and should look for a new one
    {
        FAssistUtils::RaycastMissileTarget(this,GetWorld(),
                                            PlaneMesh->GetSocketLocation("MissileMuzzle"), GetActorForwardVector(),
                                            MissileTraceLength, FirstRaytraceRadius,
                                            SecondRaytraceRadius, MissileTargetRaycastHitLocation,
                                            MissileTargetRaycastHitType);
    }
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
                MissileTarget = FAssistUtils::RaycastMissileTarget(this,GetWorld(),
                                        SpawnLocation, GetActorForwardVector(),
                                        MissileTraceLength, FirstRaytraceRadius,
                                        SecondRaytraceRadius, MissileTargetRaycastHitLocation,
                                        MissileTargetRaycastHitType);
                Missile->SetParentPawn(this);
                Missile->SetTargetOrDirection(MissileTarget, GetActorForwardVector());
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

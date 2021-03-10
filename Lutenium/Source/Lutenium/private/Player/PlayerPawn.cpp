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

    // Missile
    MissileAimTraceLength = 50000.f;
    FirstRaytraceMissileAimRadius = 20000.f;
    SecondRaytraceMissileAimRadius = 40000.f;
    AmountOfFireMissile = 1;
    MissileTargetRaycastHitType = EMissileTargetHit::NoHit;
    MissileTargetRaycastHitLocationArray.Init(FVector::ZeroVector, 2);
    MissileTargetArray.Init(nullptr, 2);

    // Upgrades
    // At the begging we don't have any upgrades
    UpgradeMap.Add(EPlayerUpgrade::IncreasedAimRadius, false);
    UpgradeMap.Add(EPlayerUpgrade::DoubleMissileAimLock, false);
    UpgradeMap.Add(EPlayerUpgrade::BaseSupport, false);
    UpgradeMap.Add(EPlayerUpgrade::Gun, false);
    UpgradeMap.Add(EPlayerUpgrade::IncreasedDyingVelocity, false);
    UpgradeMap.Add(EPlayerUpgrade::BarrelRoll, false);
    // Upgrade values
    IncreasedAimRadiusMultiplier = 1.3f;
    bHasDoubleAimLocks = false;
}

void APlayerPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    /* Raycasting every tick to update MissileTargetRaycastHitLocation so we can show the UI of the current missile target(even tho it has been shot yet) */
    MissileAimLock();
}


void APlayerPawn::MissileAimLock()
{
    IgnoredActors.Empty();
    IgnoredActors.Add(this);

    AActor* HitActor = nullptr;
    // If previos found location was a hurtbox, we wanna cache it
    /*if(MissileTargetRaycastHitType == EMissileTargetHit::MonsterWPHurtbox)
    {
        // however we need to make sure that we still can see it
        // if we don't, then look for a new one
        if(FAssistUtils::RaycastSameMonsterPosition(IgnoredActors, GetWorld(), PlaneMesh->GetSocketLocation("MissileMuzzle"), MissileTargetRaycastHitLocationArray[0]))
        {
            return;
        }
    }*/
    // If the previous found location was not hurtbox, then we don't care and should look for a new one

    // RaycastMissileTarget() Updates MissileTargetRaycastHitLocation[0] and MissileTargetRaycastHitType
    MissileTargetArray[0] = FAssistUtils::RaycastMissileTarget(IgnoredActors,GetWorld(),
                                        PlaneMesh->GetSocketLocation("MissileMuzzle"), GetActorForwardVector(),
                                        MissileAimTraceLength, FirstRaytraceMissileAimRadius,
                                        SecondRaytraceMissileAimRadius, MissileTargetRaycastHitLocationArray[0],
                                        MissileTargetRaycastHitType, HitActor);


    // Iterating through other locks in case if we have an upgrade (bHasDoubleAimLocks == true ----> IsUpgradeAquiered(EPlayerUpgrade::DoubleMissileAimLock)) == true)
    // If the first raycast hasn't found any actor, then we don't need to racyast upgraded missiles
    if(MissileTargetArray[0] != nullptr && bHasDoubleAimLocks)
    {
        for(int i = 1; i < AmountOfFireMissile; ++i)
        {
            // If previous hit actor is null, then all next raycast would be nullptr so we don't need to do them.
            // If so, we assign their locations/targets as the first raycast target
            if(HitActor == nullptr)
            {
                MissileTargetArray[i] = MissileTargetArray[0];
                MissileTargetRaycastHitLocationArray[i] = MissileTargetRaycastHitLocationArray[0];
            }
            else
            {
                IgnoredActors.Add(HitActor);
                MissileTargetArray[i] = FAssistUtils::RaycastUpgradedMissileTarget(IgnoredActors, GetWorld(),
                                                                      PlaneMesh->GetSocketLocation("MissileMuzzle"), GetActorForwardVector(),
                                                                      MissileAimTraceLength, SecondRaytraceMissileAimRadius,
                                                                      MissileTargetRaycastHitLocationArray[i], HitActor); // Updates HitActor
                if(MissileTargetArray[i] == nullptr)
                {
                    // If we found nothing, then shoot the missile to the first location
                    MissileTargetArray[i] = MissileTargetArray[0];
                    MissileTargetRaycastHitLocationArray[i] = MissileTargetRaycastHitLocationArray[0];
                }
            }
        }
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

            for(int i = 0; i < AmountOfFireMissile; ++i)
            {
                if(i < 1 || bHasDoubleAimLocks)
                {
                    AMissile* Missile = World->SpawnActor<AMissile>(MissileClass, SpawnLocation, GetActorRotation(),
                                                                    SpawnParams);
                    if (Missile)
                    {
                        Missile->SetParentPawn(this);
                        Missile->SetTargetOrDirection(MissileTargetArray[i], GetActorForwardVector());
                    }
                }
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


void APlayerPawn::UpgradePlayer(const EPlayerUpgrade NewUpgrade)
{
    if(!IsUpgradeAquiered(NewUpgrade))
    {
        switch(NewUpgrade)
        {
            /* Increased missile aim radius */
            case EPlayerUpgrade::IncreasedAimRadius:
                MissileAimTraceLength = MissileAimTraceLength * IncreasedAimRadiusMultiplier;
                FirstRaytraceMissileAimRadius = FirstRaytraceMissileAimRadius * IncreasedAimRadiusMultiplier;
                SecondRaytraceMissileAimRadius = SecondRaytraceMissileAimRadius * IncreasedAimRadiusMultiplier;
                UpgradeMap[EPlayerUpgrade::IncreasedAimRadius] = true; // At this point we are sure this key exists in map
            break;

            case EPlayerUpgrade::DoubleMissileAimLock:
                bHasDoubleAimLocks = true;
                AmountOfFireMissile = 2;
                UpgradeMap[EPlayerUpgrade::DoubleMissileAimLock] = true;
            break;

            default:

            break;
        }
    }
}

bool APlayerPawn::IsUpgradeAquiered(const EPlayerUpgrade Upgrade)
{
    bool* bHasUpgrade = UpgradeMap.Find(Upgrade);
    return bHasUpgrade != nullptr && bHasUpgrade;
}
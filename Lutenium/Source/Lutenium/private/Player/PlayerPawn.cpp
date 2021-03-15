// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Player/PlayerPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/PlayerPlaneMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Player/Missile.h"
#include "Player/MissileTargetHit.h"
#include "Kismet/KismetMathLibrary.h"
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

    PlaneMovement = CreateDefaultSubobject<UPlayerPlaneMovementComponent>(TEXT("Plane Movement"));
    PlaneMovement->SetPawn(this);
    PlaneMovement->PhysicsComponent = PlaneBox;


    // Weapons
    CurrentWeapon = EPlayerWeapon::Missile;

    // Missile
    MissileAimTraceLength = 50000.f;
    FirstRaytraceMissileAimRadius = 20000.f;
    SecondRaytraceMissileAimRadius = 40000.f;
    UpgradeRaytraceMissileAimRadius = 10000.f;
    AmountOfFireMissile = 1;
    MissileTargetRaycastHitType = EMissileTargetHit::NoHit;
    MissileTargetRaycastHitLocationArray.Init(FVector::ZeroVector, 2);
    MissileTargetArray.Init(nullptr, 2);

    // Machine Gun
    MachineGunFireRate = 0.3f;

    // Upgrades
    // At the begging we don't have any upgrades
    UpgradeMap.Add(EPlayerUpgrade::IncreasedAimRadius, false);
    UpgradeMap.Add(EPlayerUpgrade::DoubleMissileAimLock, false);
    UpgradeMap.Add(EPlayerUpgrade::BaseSupport, false);
    UpgradeMap.Add(EPlayerUpgrade::MachineGun, false);
    UpgradeMap.Add(EPlayerUpgrade::IncreasedDyingVelocity, false);
    UpgradeMap.Add(EPlayerUpgrade::BarrelRoll, false);
    UpgradeMap.Add(EPlayerUpgrade::BackMirrors, false);

    // Upgrade values
    IncreasedAimRadiusMultiplier = 1.3f;
    bHasDoubleAimLocks = false;
}

void APlayerPawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    /* Raycasting every tick to update MissileTargetRaycastHitLocation so we can show the UI of the current missile target(even tho it has been shot yet) */
    if(CurrentWeapon == EPlayerWeapon::Missile)
    {
        MissileAimLock();
    }
}


void APlayerPawn::MissileAimLock()
{
    IgnoredActors.Empty();
    IgnoredActors.Add(this);

    AActor* HitActor = nullptr;

    // RaycastMissileTarget() Updates MissileTargetRaycastHitLocation[0] and MissileTargetRaycastHitType
    MissileTargetArray[0] = FAssistUtils::RaycastMissileTarget(IgnoredActors,GetWorld(),
                                        PlaneMesh->GetSocketLocation("MissileMuzzle"), GetActorForwardVector(),
                                        MissileAimTraceLength, FirstRaytraceMissileAimRadius,
                                        SecondRaytraceMissileAimRadius, MissileTargetRaycastHitLocationArray[0],
                                        MissileTargetRaycastHitType, HitActor);


    // Iterating through other locks in case if we have an upgrade (bHasDoubleAimLocks == true ----> IsUpgradeAquiered(EPlayerUpgrade::DoubleMissileAimLock)) == true)
    if(bHasDoubleAimLocks)
    {
        for(int i = 1; i < AmountOfFireMissile; ++i)
        {
            // If previous hit actor is null, then all next raycast would be nullptr so we don't need to do them.
            // If so, we assign their locations/targets as the first raycast target
            if(HitActor == nullptr || MissileTargetRaycastHitType != EMissileTargetHit::MonsterWPHurtbox)
            {
                MissileTargetArray[i] = MissileTargetArray[0];
                MissileTargetRaycastHitLocationArray[i] = MissileTargetRaycastHitLocationArray[0];
            }
            else
            {
                IgnoredActors.Add(HitActor);
                MissileTargetArray[i] = FAssistUtils::RaycastUpgradedMissileTarget(IgnoredActors, GetWorld(),
                                                                      PlaneMesh->GetSocketLocation("MissileMuzzle"), GetActorForwardVector(),
                                                                      MissileAimTraceLength, UpgradeRaytraceMissileAimRadius,
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
    PlayerInputComponent->BindAxis("Thrust", PlaneMovement, &UPlayerPlaneMovementComponent::ThrustInput);
    PlayerInputComponent->BindAxis("Pitch", PlaneMovement, &UPlayerPlaneMovementComponent::PitchInput);
    PlayerInputComponent->BindAxis("Yawn", PlaneMovement, &UPlayerPlaneMovementComponent::YawnInput);
    PlayerInputComponent->BindAxis("Roll", PlaneMovement, &UPlayerPlaneMovementComponent::RollInput);
    PlayerInputComponent->BindAction("Stop", IE_Released, PlaneMovement, &UPlayerPlaneMovementComponent::DashInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerPawn::WeaponInputPressed);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerPawn::WeaponInputReleased);
}


void APlayerPawn::WeaponInputPressed()
{
    bWeaponFiring = true;
    if(CurrentWeapon == EPlayerWeapon::Missile)
    {
        FireMissile();
    }
    else
    {
        // Deactivating HUD for aim lock
        MissileTargetRaycastHitType = EMissileTargetHit::NoHit;
        // Starting firign da maching gun
        GetWorld()->GetTimerManager().SetTimer(MachingGunTimerHandler, this, &APlayerPawn::FireMachineGun, MachineGunFireRate,
                                       true);
    }
}


void APlayerPawn::WeaponInputReleased()
{
    bWeaponFiring = false;
    if(CurrentWeapon == EPlayerWeapon::MachineGun)
    {
        GetWorld()->GetTimerManager().ClearTimer(MachingGunTimerHandler);
    }
}


void APlayerPawn::SwitchWeapons(EPlayerWeapon NextWeapon)
{

    // if we don't have an upgrade for machine gun and the next weapon is machine gun
    if(NextWeapon == EPlayerWeapon::MachineGun && !bHasMachineGun)
    {
        return;
    }

    if(CurrentWeapon != NextWeapon)
    {
        CurrentWeapon = NextWeapon;
        OnSwitchWeaponEvent(NextWeapon);
    }
}

void APlayerPawn::FireMachineGun()
{
    // safety checks
    if(MachineGunBulletClass == nullptr)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = this;

    FVector SpawnLocation = PlaneMesh->GetSocketLocation("MissileMuzzle");
    AActor* Bullet = World->SpawnActor<AMissile>(MissileClass, SpawnLocation, UKismetMathLibrary::MakeRotFromX(GetActorForwardVector()),
                                                            SpawnParams);
    OnFireMachineGunEvent();
}

void APlayerPawn::FireMissile()
{
    // safety checks
    if (MissileClass == nullptr)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        return;
    }


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
                Missile->SetTargetOrDirection(MissileTargetArray[i], GetActorForwardVector());
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
            break;

            case EPlayerUpgrade::DoubleMissileAimLock:
                bHasDoubleAimLocks = true;
                AmountOfFireMissile = 2;
            break;

            case EPlayerUpgrade::MachineGun:
                bHasMachineGun = true;
            break;

            default:

            break;
        }
        if(UpgradeMap.Find(NewUpgrade) != nullptr)
        {
            UpgradeMap[NewUpgrade] = true;
        }
        else
        {
            UpgradeMap.Add(NewUpgrade, true);
        }
        AquieredUpgrade(NewUpgrade);
    }
}

bool APlayerPawn::IsUpgradeAquiered(const EPlayerUpgrade Upgrade)
{
    bool* bHasUpgrade = UpgradeMap.Find(Upgrade);
    return bHasUpgrade != nullptr && bHasUpgrade;
}
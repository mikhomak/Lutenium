#include "Monster/EnemyMonsterPawn.h"
#include "Monster/MonsterLeg.h"
#include "Monster/MonsterLegComponent.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/PipeMW.h"
#include "Monster/Weapons/SirenMW.h"
#include "Monster/Weapons/FanMW.h"
#include "Monster/Weapons/TrafficLightMW.h"
#include "Monster/Weapons/PowerSystemMW.h"
#include "Player/PlayerPawn.h"
#include "Monster/MonsterAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

AEnemyMonsterPawn::AEnemyMonsterPawn()
{
    /* setting the tick values. The tick group is PostPhysics */
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = ETickingGroup::TG_PostPhysics;

    /* Setting the attachment rules */
    const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
        EAttachmentRule::KeepRelative, true);

    /* Setting the root component as a sphere */
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
    SphereComponent->SetSimulatePhysics(true);
    SphereComponent->SetEnableGravity(false);
    RootComponent = SphereComponent;

    /* Setting the mesh */
    MonsterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    MonsterMesh->AttachToComponent(SphereComponent, AttachmentTransformRules);

    /* Setting Health settings */
    Health = 500.f;
    DirectDamageReduction = 0.5f;
    bHandleDeathInCpp = true;

    /* Setting up body variables */
    BodySocketName = "BodySocket";
    BodyUpMovementSpeed = 1000.f;

    /* Setting the movement  */
    PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Floating Movement"));

    /* Creates sphere for the beam defense */
    BeamDefenseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Beam Defense Sphere"));
    BeamDefenseSphere->AttachToComponent(SphereComponent, AttachmentTransformRules);

    /* Initialize legs */
    RearLeftLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Rear Left Leg"));
    RearLeftLeg->SetEnemyMonsterPawn(this);
    RearLeftLeg->SetMonsterMesh(MonsterMesh);
    RearLeftLeg->SetMonsterLegType(EMonsterLeg::RearLeft);
    Legs.Add(RearLeftLeg);

    RearRightLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Rear Right Leg"));
    RearRightLeg->SetEnemyMonsterPawn(this);
    RearRightLeg->SetMonsterMesh(MonsterMesh);
    RearRightLeg->SetMonsterLegType(EMonsterLeg::RearRight);
    Legs.Add(RearRightLeg);

    FrontLeftLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Front Left Leg"));
    FrontLeftLeg->SetEnemyMonsterPawn(this);
    FrontLeftLeg->SetMonsterMesh(MonsterMesh);
    FrontLeftLeg->SetMonsterLegType(EMonsterLeg::FrontLeft);
    Legs.Add(FrontLeftLeg);

    FrontRightLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Front Right Leg"));
    FrontRightLeg->SetEnemyMonsterPawn(this);
    FrontRightLeg->SetMonsterMesh(MonsterMesh);
    FrontRightLeg->SetMonsterLegType(EMonsterLeg::FrontRight);
    Legs.Add(FrontRightLeg);

    ToggleWhatLegsShouldMove(true);

    /* Setting weapons sockets */
    PipeSocketName = "PipeSocket";
    FanSocketName = "FanSocket";
    SirenSocketName = "SirenSocket";
    TrafficLightSocketName = "TrafficLightSocket";
    PowerSystemSocketName = "PowerSystemSocket";

    /* Don't forget to set the AI Controller in Blueprint! */
}

void AEnemyMonsterPawn::BeginPlay()
{
    Super::BeginPlay();
    SpawnWeapons();
    /** Setting MonsterAI, in case it wasn't set */
    if(MonsterAI == nullptr)
    {
        AMonsterAIController* AIContr = Cast<AMonsterAIController>(GetController());
        if(AIContr)
        {
            MonsterAI = AIContr;
        }
    }
}

void AEnemyMonsterPawn::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}


void AEnemyMonsterPawn::SpawnWeapons()
{

    /* Spawn Params */
    FActorSpawnParameters SpawnParams;

    SpawnParams.Owner = this;
    SpawnParams.Instigator = this;


    /* PIPE */
    if(PipeClass)
    {
        APipeMW* Pipe = GetWorld()->SpawnActor<APipeMW>(PipeClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if(Pipe)
        {
            Pipe->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), PipeSocketName);
            Pipe->MonsterPawn = this;
            Pipe->MonsterMesh = MonsterMesh;
            WeaponMap.Add(EMonsterWeaponType::Pipe, Pipe);
        }
    }

    /* FAN */
    if(FanClass)
    {
        AFanMW* Fan = GetWorld()->SpawnActor<AFanMW>(FanClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if(Fan)
        {
            Fan->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), FanSocketName);
            Fan->MonsterPawn = this;
            Fan->MonsterMesh = MonsterMesh;
            WeaponMap.Add(EMonsterWeaponType::Fan, Fan);
        }
    }

    /* SIREN */
    if(SirenClass)
    {
        ASirenMW* Siren = GetWorld()->SpawnActor<ASirenMW>(SirenClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if(Siren)
        {
            Siren->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), SirenSocketName);
            Siren->MonsterPawn = this;
            Siren->MonsterMesh = MonsterMesh;
            WeaponMap.Add(EMonsterWeaponType::Siren, Siren);
        }
    }

    /* TRAFFIC LIGHT */
    if(TrafficLightClass)
    {
        ATrafficLightMW* TrafficLight = GetWorld()->SpawnActor<ATrafficLightMW>(TrafficLightClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if(TrafficLight)
        {
            TrafficLight->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), TrafficLightSocketName);
            TrafficLight->MonsterPawn = this;
            TrafficLight->MonsterMesh = MonsterMesh;
            WeaponMap.Add(EMonsterWeaponType::TrafficLight, TrafficLight);
        }
    }

    /* POWER SYSTEM */
    if(PowerSystemClass)
    {
        APowerSystemMW* PowerSystem = GetWorld()->SpawnActor<APowerSystemMW>(PowerSystemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if(PowerSystem)
        {
            PowerSystem->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), PowerSystemSocketName);
            PowerSystem->MonsterPawn = this;
            PowerSystem->MonsterMesh = MonsterMesh;
            PowerSystem->InitializeTowerFencse(); // do it after setting up monster pawn and mesh
            WeaponMap.Add(EMonsterWeaponType::PowerSystem, PowerSystem);
        }
    }

}



/* Setting actor eyes view point so AI CONTROLLER will put perception on the body  */
void AEnemyMonsterPawn::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
    Location = MonsterMesh->GetSocketLocation(BodySocketName);
    Rotation = GetActorRotation();
}

void AEnemyMonsterPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    bIsBodyMovingUp = RaycastLegJoints();
    if (bIsBodyMovingUp)
    {
        PawnMovement->AddInputVector(FVector::UpVector, true);
    }
}

/*
 * Get leg location to use in animation blueprint
 */
FVector AEnemyMonsterPawn::GetLegLocation(const EMonsterLeg Leg) const
{
    switch (Leg)
    {
    case EMonsterLeg::FrontLeft:
        return FrontLeftLeg->GetCurrentPosition();
    case EMonsterLeg::FrontRight:
        return FrontRightLeg->GetCurrentPosition();
    case EMonsterLeg::RearLeft:
        return RearLeftLeg->GetCurrentPosition();
    case EMonsterLeg::RearRight:
        return RearRightLeg->GetCurrentPosition();
    }
    return FVector();
}

void AEnemyMonsterPawn::LegHasMovedEventCaller(const EMonsterLeg MonsterLeg)
{
    ToggleWhatLegsShouldMove(MonsterLeg == EMonsterLeg::FrontLeft || MonsterLeg == EMonsterLeg::RearRight);
    LegHasMoved(MonsterLeg);
}

bool AEnemyMonsterPawn::RaycastLegJoints()
{
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(this);
    for (UMonsterLegComponent* LegComp : Legs)
    {
        const FVector FirstJoint = MonsterMesh->GetSocketLocation(LegComp->FirstJointSocket);
        const FVector SecondJoint = MonsterMesh->GetSocketLocation(LegComp->SecondJointSocket);

        GetWorld()->LineTraceSingleByChannel(
            HitResult,
            FirstJoint,
            SecondJoint,
            ECollisionChannel::ECC_WorldStatic,
            CollisionParams);
        if (HitResult.bBlockingHit)
        {
            return true;
        }
    }
    return false;
}

void AEnemyMonsterPawn::BodyTimelineMovement()
{
}

void AEnemyMonsterPawn::BodyTimelineMovementFinish()
{
}

/* Checks if there is something between body and the first joint of each leg */
/* If so, start timeline to move the body up*/
void AEnemyMonsterPawn::CheckBodyAltitudeDependingOnLegs()
{
    const FVector BodySocketLocation = MonsterMesh->GetSocketLocation(BodySocketName);
    for (const auto SocketName : TopSocketLocationNames)
    {
        const FVector SocketLocation = MonsterMesh->GetSocketLocation(SocketName);
        FHitResult HitResult;

        GetWorld()->LineTraceSingleByChannel(
            HitResult,
            BodySocketLocation,
            SocketLocation,
            ECollisionChannel::ECC_WorldStatic);

        if (HitResult.bBlockingHit && !bBodyMoving)
        {
            BodyTimeline.PlayFromStart();
            bBodyMoving = true;
        }
    }
}

void AEnemyMonsterPawn::ToggleWhatLegsShouldMove(const bool Left) const
{
    FrontLeftLeg->SetCanMove(!Left);
    RearRightLeg->SetCanMove(!Left);
    FrontRightLeg->SetCanMove(Left);
    RearLeftLeg->SetCanMove(Left);
}

/** Removes a weapon when it got destroyed */
void AEnemyMonsterPawn::LooseWeapon(EMonsterWeaponType WeaponType)
{
    WeaponMap.FindAndRemoveChecked(WeaponType);
}


float AEnemyMonsterPawn::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
    Damage *= DirectDamageReduction;
    Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    TakeNonDirectDamage(Damage);
    return Damage;
}


void AEnemyMonsterPawn::TakeNonDirectDamage(float Damage)
{
    Health -= Damage;
    if(Health < 0)
    {
        Die();
    }
}

void AEnemyMonsterPawn::Die()
{
    OnDieEvent();
    if(bHandleDeathInCpp)
    {
        Destroy();
    }
}

/** Player has entered the radius of beam defense */
/** Telling that to MonsterAIController and we will do something with it in behaviour tree*/
void AEnemyMonsterPawn::PlayerHasEnteredBeamDefense(AActor* OverlapActor)
{
    APlayerPawn* Player = Cast<APlayerPawn>(OverlapActor);
    if(Player && MonsterAI)
    {
        MonsterAI->SetIsPlayerInRadiusOfBeamDefense(true);
    }
}

/** Player has exit beam defense radius */
/** Notiyfing monster ai controller */
void AEnemyMonsterPawn::PlayerHasExitdBeamDefense(AActor* OverlapExitActor)
{
    APlayerPawn* Player = Cast<APlayerPawn>(OverlapActor);
    if(Player && MonsterAI)
    {
        MonsterAI->SetIsPlayerInRadiusOfBeamDefense(false);
    }
}

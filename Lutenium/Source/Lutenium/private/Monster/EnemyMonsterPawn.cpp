#include "../../public/Monster/EnemyMonsterPawn.h"
#include "../../public/Monster/MonsterLeg.h"
#include "../../public/Monster/MonsterLegComponent.h"
#include "../../public/Monster/MonsterWeapon.h"
#include "../../public/Monster/Weapons/PipeMW.h"
#include "../../public/Monster/Weapons/SirenMW.h"
#include "../../public/Monster/Weapons/SputnikMW.h"
#include "../../public/Monster/Weapons/TrafficLightMW.h"
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

    /* Setting up body variables */
    BodySocketName = "BodySocket";
    BodyUpMovementSpeed = 1000.f;

    /* Setting the movement  */
    PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Floating Movement"));

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

    PipeSocketName = "PipeSocket";
    SputnikSocketName = "SputnikSocket";
    SirenSocketName = "SirenSocket";
    TrafficLightSocketName = "TrafficLightSocket";

    /* Don't forget to set the AI Controller in Blueprint! */
}

void AEnemyMonsterPawn::BeginPlay()
{
    Super::BeginPlay();
    SpawnWeapons();
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
        APipeMW* SpawnedPipe = GetWorld()->SpawnActor<APipeMW>(PipeClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if(SpawnedPipe)
        {
            Pipe = SpawnedPipe;
            Pipe->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), PipeSocketName);
            Pipe->MonsterPawn = this;
            Pipe->MonsterMesh = MonsterMesh;
            Weapons.Add(Pipe);
        }
    }

    /* SPUTNIK */
    if(SputnikClass)
    {
        ASputnikMW* SpawnedSputnik = GetWorld()->SpawnActor<ASputnikMW>(SputnikClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if(SpawnedSputnik)
        {
            Sputnik = SpawnedSputnik;
            Sputnik->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), SputnikSocketName);
            Sputnik->MonsterPawn = this;
            Sputnik->MonsterMesh = MonsterMesh;
            Weapons.Add(Sputnik);
        }
    }

    /* SIREN */
    if(SirenClass)
    {
        ASirenMW* SpawnedSiren = GetWorld()->SpawnActor<ASirenMW>(SirenClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if(SpawnedSiren)
        {
            Siren = SpawnedSiren;
            Siren->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), SirenSocketName);
            Siren->MonsterPawn = this;
            Siren->MonsterMesh = MonsterMesh;
            Weapons.Add(Siren);
        }
    }

    /* TRAFFIC LIGHT */
    if(TrafficLightClass)
    {
        ATrafficLightMW* SpawnedTrafficLight = GetWorld()->SpawnActor<ATrafficLightMW>(TrafficLightClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        if(SpawnedTrafficLight)
        {
            TrafficLight = SpawnedTrafficLight;
            TrafficLight->AttachToComponent(MonsterMesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false), TrafficLightSocketName);
            TrafficLight->MonsterPawn = this;
            TrafficLight->MonsterMesh = MonsterMesh;
            Weapons.Add(TrafficLight);
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


void AEnemyMonsterPawn::OnTakeDamage(float Damage)
{
    Health -= Damage * ArmourDamageReduction;
    // TODO win and shit
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

void AEnemyMonsterPawn::LooseWeapon(AMonsterWeapon* LostWeapon)
{


    UClass* WeaponClass = LostWeapon->GetClass();

    // Removing the reference of the weapon
    if (WeaponClass == APipeMW::StaticClass())
    {
        Pipe = nullptr;
    }
    else if (WeaponClass == ASputnikMW::StaticClass())
    {
        Sputnik = nullptr;
    }
    else if(WeaponClass == ATrafficLightMW::StaticClass())
    {
        TrafficLight = nullptr;
    }
    else if(WeaponClass == ASirenMW::StaticClass())
    {
        Siren = nullptr;
    }

    if(Weapons.Num() != 0)
    {

        Weapons.Remove(LostWeapon);

        for (AMonsterWeapon* Weapon : Weapons)
        {
            Weapon->UpgradeWeapon();
        }
    }
}

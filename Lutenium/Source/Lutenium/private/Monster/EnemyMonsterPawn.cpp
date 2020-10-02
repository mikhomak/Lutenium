#include "../../public/Monster/EnemyMonsterPawn.h"
#include "../../public/Monster/MonsterLeg.h"
#include "../../public/Monster/MonsterLegComponent.h"
#include "../../public/Monster/MonsterWeapon.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"


AEnemyMonsterPawn::AEnemyMonsterPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    MonsterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    MonsterMesh->SetEnableGravity(false);
    MonsterMesh->SetSimulatePhysics(true);
    MonsterMesh->SetTickGroup(TG_PostPhysics);
    RootComponent = MonsterMesh;


    /* Initialize legs */
    RearLeftLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Rear Left Leg"));
    RearLeftLeg->SetEnemyMonsterPawn(this);
    RearLeftLeg->SetMonsterLegType(RearLeft);

    RearRightLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Rear Right Leg"));
    RearRightLeg->SetEnemyMonsterPawn(this);
    RearRightLeg->SetMonsterLegType(RearRight);

    FrontLeftLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Front Left Leg"));
    FrontLeftLeg->SetEnemyMonsterPawn(this);
    FrontLeftLeg->SetMonsterLegType(FrontLeft);

    FrontRightLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Front Right Leg"));
    FrontRightLeg->SetEnemyMonsterPawn(this);
    FrontRightLeg->SetMonsterLegType(FrontRight);

    ToggleWhatLegsShouldMove(true);

    /* Don't forget to set the Controller in Blueprint! */

    /* Initialize weapons*/
    MonsterWeapon = CreateDefaultSubobject<UMonsterWeapon>(TEXT("Monster weapons"));
    MonsterWeapon->SetMonsterMesh(MonsterMesh);
    MonsterWeapon->SetMonsterPawn(this);
}

void AEnemyMonsterPawn::BeginPlay()
{
    Super::BeginPlay();

}


void AEnemyMonsterPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

/*
 * Get leg location to use in animation blueprint
 */
FVector AEnemyMonsterPawn::GetLegLocation(const EMonsterLeg Leg) const
{
    switch (Leg)
    {
    case FrontLeft:
        return FrontLeftLeg->GetCurrentPosition();
    case FrontRight:
        return FrontRightLeg->GetCurrentPosition();
    case RearLeft:
        return RearLeftLeg->GetCurrentPosition();
    case RearRight:
        return RearRightLeg->GetCurrentPosition();
    }
    return FVector();
}

void AEnemyMonsterPawn::LegHasMovedEventCaller(const EMonsterLeg MonsterLeg)
{
    ToggleWhatLegsShouldMove(MonsterLeg == FrontLeft || MonsterLeg == RearRight);
    LegHasMoved(MonsterLeg);
}


float AEnemyMonsterPawn::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                    AActor* DamageCauser)
{
    FHitResult HitResult;
    FVector ImpulseDir;
    DamageEvent.GetBestHitInfo(this, DamageCauser, HitResult, ImpulseDir);
    ImpulseDir.Normalize();

    MissileCollide(HitResult.Location, ImpulseDir, Damage);

    return Damage;
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

void AEnemyMonsterPawn::DoScream()
{

}
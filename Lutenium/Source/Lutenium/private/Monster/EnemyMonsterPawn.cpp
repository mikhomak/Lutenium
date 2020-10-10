#include "../../public/Monster/EnemyMonsterPawn.h"
#include "../../public/Monster/MonsterLeg.h"
#include "../../public/Monster/MonsterLegComponent.h"
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
    
    BodySocketName = "BodySocket";

    /* Setting the movement  */
    PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Pawn Floating Movement"));
    
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

}

void AEnemyMonsterPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyMonsterPawn::PostInitializeComponents()
{
    Super::PostInitializeComponents();
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

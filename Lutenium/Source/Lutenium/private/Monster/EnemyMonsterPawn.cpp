#include "../../public/Monster/EnemyMonsterPawn.h"
#include "../../public/Monster/MonsterLeg.h"

AEnemyMonsterPawn::AEnemyMonsterPawn()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemyMonsterPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyMonsterPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

FVector AEnemyMonsterPawn::CalculateLegPosition(const EMonsterLeg Leg)
{
    FVector* CurrentPosition = nullptr;
    FTimeline* CurrentTimeline;
    FVector* CurrentRaycastPosition = nullptr;

    switch (Leg)
    {
    case FrontRight:
        CurrentPosition = &CurrentFrontRightPosition;
        CurrentTimeline = &FrontRightLegTimeline;
        CurrentRaycastPosition = &RaycastFrontRightPosition;
        break;
    case FrontLeft:
        CurrentPosition = &CurrentFrontLeftPosition;
        CurrentTimeline = &FrontLeftLegTimeline;
        CurrentRaycastPosition = &RaycastFrontLeftPosition;
        break;
    case RearRight:
        CurrentPosition = &CurrentFrontRightPosition;
        CurrentTimeline = &FrontRightLegTimeline;
        CurrentRaycastPosition = &RaycastFrontRightPosition;
        break;
    case RearLeft:
        CurrentPosition = &CurrentFrontLeftPosition;
        CurrentTimeline = &FrontLeftLegTimeline;
        CurrentRaycastPosition = &RaycastFrontLeftPosition;
        break;
    }
    FHitResult HitResult;
    const FVector RaycastStart = *CurrentRaycastPosition;
    const FVector RaycastEndLocation = RaycastStart - (FVector::DownVector * RaycastDownLength);

    GetWorld()->LineTraceSingleByChannel(
        HitResult,
        RaycastStart,
        RaycastEndLocation,
        ECollisionChannel::ECC_WorldDynamic);

    FVector HitLocation = HitResult.bBlockingHit ? HitResult.ImpactPoint : FVector();

    if (FVector::Distance(*CurrentPosition, HitLocation) >= DistanceBetweenLegsToMove)
    {
        
    }

    return FVector(0, 0, 0);
}



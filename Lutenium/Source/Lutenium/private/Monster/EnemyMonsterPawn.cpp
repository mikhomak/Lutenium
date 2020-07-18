#include "../../public/Monster/EnemyMonsterPawn.h"
#include "../../public/Monster/MonsterLeg.h"
#include "../../public/Monster/MonsterLegComponent.h"
#include "Components/SkeletalMeshComponent.h"


AEnemyMonsterPawn::AEnemyMonsterPawn()
{
    PrimaryActorTick.bCanEverTick = true;


    MonsterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    MonsterMesh->SetEnableGravity(false);
    MonsterMesh->SetSimulatePhysics(true);
    MonsterMesh->SetTickGroup(TG_PostPhysics);
    RootComponent = MonsterMesh;
    
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
}

void AEnemyMonsterPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyMonsterPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

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
    LegHasMoved(MonsterLeg);
}

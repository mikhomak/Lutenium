#include "../../public/Monster/EnemyMonsterPawn.h"
#include "../../public/Monster/MonsterLeg.h"
#include "../../public/Monster/MonsterLegComponent.h"

AEnemyMonsterPawn::AEnemyMonsterPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    RearLeftLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Rear Left Leg"));
    RearLeftLeg->SetEnemyMonsterPawn(this);
    
    RearRightLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Rear Right Leg"));
    RearRightLeg->SetEnemyMonsterPawn(this);

    FrontLeftLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Front Left Leg"));
    FrontLeftLeg->SetEnemyMonsterPawn(this);
    
    FrontRightLeg = CreateDefaultSubobject<UMonsterLegComponent>(TEXT("Front Right Leg"));
    FrontRightLeg->SetEnemyMonsterPawn(this);
}

void AEnemyMonsterPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyMonsterPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

FVector AEnemyMonsterPawn::GetLegLocation(EMonsterLeg Leg)
{
    return FVector();
}

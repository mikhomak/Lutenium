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

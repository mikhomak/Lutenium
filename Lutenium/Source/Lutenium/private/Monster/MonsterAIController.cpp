#include "Monster/MonsterAIController.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/PipeMW.h"
#include "Monster/Weapons/SirenMW.h"
#include "Monster/Weapons/FanMW.h"
#include "Monster/Weapons/TrafficLightMW.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AMonsterAIController::AMonsterAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

    /* Initialize sensing */
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    /* Add event OnSeePlayer in BP */
}

void AMonsterAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AMonsterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
void AMonsterAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    AEnemyMonsterPawn* Monster = Cast<AEnemyMonsterPawn>(InPawn);

    if (Monster && Monster->BehaviorTree->BlackboardAsset)
    {
        MonsterPawn = Monster;
        BlackboardComp->InitializeBlackboard(*Monster->BehaviorTree->BlackboardAsset);
        BehaviorComp->StartTree(*Monster->BehaviorTree);
        MonsterMesh = Monster->MonsterMesh;
    }
}

void AMonsterAIController::OnSeePlayer(TArray<AActor*> Actors)
{

}

AMonsterWeapon* AMonsterAIController::GetWeapon(EMonsterWeaponType MonsterWeaponType)
{
    AMonsterWeapon* Weapon = nullptr;
    switch(MonsterWeaponType)
    {
        case EMonsterWeaponType::Pipe:
            Weapon = MonsterPawn->Pipe;
        break;
        case EMonsterWeaponType::Fan:
            Weapon = MonsterPawn->Fan;
        break;
        case EMonsterWeaponType::TrafficLight:
            Weapon = MonsterPawn->TrafficLight;
        break;
        case EMonsterWeaponType::Siren:
            Weapon = MonsterPawn->Siren;
        break;
    }

    return Weapon;
}

int32 AMonsterAIController::GetWeaponLevel(EMonsterWeaponType MonsterWeaponType)
{
    AMonsterWeapon* Weapon = GetWeapon(MonsterWeaponType);
    if(Weapon != nullptr)
    {
        return Weapon->GetUpgradeWeapon();
    }
    return -1;
}
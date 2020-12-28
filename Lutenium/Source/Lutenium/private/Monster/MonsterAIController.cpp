#include "Monster/MonsterAIController.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Player/PlayerPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"

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

    /* Set up names for blackboard values */
    FN_BV_bIsPlayerInRadiusOfBeamDefense = "bIsPlayerInRadiusOfBeamDefense";
    FN_BV_PlayerHightLevel = "PlayerHightLevel";
}

void AMonsterAIController::BeginPlay()
{
    Super::BeginPlay();
    /* Storring player referense */
    APlayerPawn* PlayerCharacter = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    if(PlayerCharacter)
    {
        Player = PlayerCharacter;
    }
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

/** IMPORTANT!!! Could ne null!!! */
AMonsterWeapon* AMonsterAIController::GetWeapon(EMonsterWeaponType MonsterWeaponType)
{
    if(MonsterPawn)
    {
        return MonsterPawn->WeaponMap.FindRef(MonsterWeaponType);
    }
    return nullptr;
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

/** Setting blackboard value for PlayerInRadiusOfBeamDefense */
void AMonsterAIController::SetIsPlayerInRadiusOfBeamDefense(bool bIsInRadius)
{
    bIsPlayerInRadiusOfBeamDefense = bIsInRadius;
    BlackboardComp->SetValueAsBool(FN_BV_bIsPlayerInRadiusOfBeamDefense, bIsInRadius);
}

/** Setting tha blackboard value for the PlayerHightLevel */
int32 AMonsterAIController::SetPlayerHightLevelBlackboardValue()
{
    const float PlayerHight = Player->GetActorLocation().Z;
    int32 Hight = 2; /** by default it should be center */
    /* indexes of hight level socket location star with 1! */
    if(MonsterPawn->GetHightLevelSocketLocation(1).Z < PlayerHight)
    {
        Hight = 1;
    }
    else if(MonsterPawn->GetHightLevelSocketLocation(3).Z > PlayerHight)
    {
        Hight = 3;
    }
    BlackboardComp->SetValueAsInt(FN_BV_PlayerHightLevel, Hight);
    return Hight;
}
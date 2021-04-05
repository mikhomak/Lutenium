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


    /* Set up names for blackboard values */
    FN_BV_bIsPlayerInRadiusOfBeamDefense = "Is Player In Radius Of Beam Defense?";
    FN_BV_PlayerHightLevel = "Player Hight Level";
    FN_BV_CurrentTargetIndex = "Current target index to reach";
    FN_BV_bHasReachedCurrentTarget = "Has Monster reached the current target?";
    FN_BV_bIsMonsterMoving = "Is monster moving?";
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
    if(Player == nullptr)
    {
        return -1;
    }

    const float PlayerHight = Player->GetActorLocation().Z;
    int32 Hight = 2; /** by default it should be center */
    /* indexes of hight level socket location star with 1! */
    const FVector FirstHight = MonsterPawn->GetHightLevelSocketLocation(1);
    const FVector SecondHight = MonsterPawn->GetHightLevelSocketLocation(2);
    const FVector FourthHight = MonsterPawn->GetHightLevelSocketLocation(4);
    if(FirstHight.Z > PlayerHight)
    {
        Hight = 1;
    }
    else if(FirstHight.Z < PlayerHight && SecondHight.Z > PlayerHight)
    {
        Hight = 2;
    }
    else if(SecondHight.Z < PlayerHight && FourthHight.Z > PlayerHight)
    {
        Hight = 3;
    }
    else
    {
        Hight = 4;
    }
    BlackboardComp->SetValueAsInt(FN_BV_PlayerHightLevel, Hight);
    return Hight;
}


void AMonsterAIController::SetTargetsAndNextPosition(TArray<class AActor*>& NewTargets)
{
    Targets = NewTargets;
    if(Targets.IsValidIndex(0))
    {
        CurrentTargetIndex = 0;
        NextTargetPositionWS = Targets[0]->GetActorLocation();
    }
}

void AMonsterAIController::StartMovingToNextTargetLocation(int32 Index, bool bShouldMove)
{
    if(Targets.IsValidIndex(Index))
    {
        CurrentTargetIndex = Index;        
        NextTargetPositionWS = Targets[Index]->GetActorLocation();
        MonsterPawn->SetDirectionToMove(NextTargetPositionWS);
        BlackboardComp->SetValueAsInt(FN_BV_CurrentTargetIndex, Index);
        // Starts or stops movement
        StartOrStopMoving(bShouldMove);
    }
}


void AMonsterAIController::SetHasReachedCurrentTarget(bool bHasReached)
{
    BlackboardComp->SetValueAsBool(FN_BV_bHasReachedCurrentTarget, bHasReached);
}


void AMonsterAIController::StartOrStopMoving(bool bStartOrStopMoving)
{
    bCanMove = bStartOrStopMoving;
    BlackboardComp->SetValueAsBool(FN_BV_bHasReachedCurrentTarget, bStartOrStopMoving);
}
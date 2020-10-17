// Fill out your copyright notice in the Description page of Project Settings.


#include "../../public/Monster/MonsterAIController.h"
#include "../../public/Monster/EnemyMonsterPawn.h"
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

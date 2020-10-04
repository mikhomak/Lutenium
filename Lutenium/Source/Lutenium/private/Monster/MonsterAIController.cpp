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
    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

    /* Initialize sensing */
    PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    if (SightConfig)
    {
        SightConfig->SightRadius = SightRadius;
        SightConfig->LoseSightRadius = SightLooseRadius;
        SightConfig->PeripheralVisionAngleDegrees = SightAngle;
    }
    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
    /* Add event OnSeePlayer in BP */
}

void AMonsterAIController::BeginPlay()
{
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
    }
}

void AMonsterAIController::OnSeePlayer(TArray<AActor*> Actors)
{
    MonsterPawn->DoScream();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "../../public/Monster/MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Monster/EnemyMonsterPawn.h"

AMonsterAIController::AMonsterAIController()
{
    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree"));

    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}


void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::Possess(InPawn);

	AEnemyMonsterPawn* Monster = Cast<AEnemyMonsterPawn>(InPawn);

	if (Monster && Monster->BehaviorTree->BlackboardAsset)
	{
		BlackboardComp->InitializeBlackboard(*Monster->BehaviorTree->BlackboardAsset);
		GLog->Log("Initialized blackboard.");
		BehaviorComp->StartTree(*Monster->BehaviorTree);

	}

}
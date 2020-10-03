// Fill out your copyright notice in the Description page of Project Settings.


#include "../../public/Monster/MonsterAIController.h"
#include "../../public/Monster/EnemyMonsterPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"


AMonsterAIController::AMonsterAIController()
{
    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree"));
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	/* Initialize sensing */
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;
}

void AMonsterAIController::BeginPlay()
{
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AMonsterAIController::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AMonsterAIController::OnHearNoise);
	}
}

void AMonsterAIController::OnSeePlayer(APawn* SeenPawn)
{

}

void AMonsterAIController::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::Possess(InPawn);
	AEnemyMonsterPawn* Monster = Cast<AEnemyMonsterPawn>(InPawn);

	if (Monster && Monster->BehaviorTree->BlackboardAsset)
	{
		MonsterPawn = Monster;
		BlackboardComp->InitializeBlackboard(*Monster->BehaviorTree->BlackboardAsset);
		BehaviorComp->StartTree(*Monster->BehaviorTree);
	}

}

#include "Monster/Tasks/BTTask_UpdateNextTargetLocation.h"
#include "Monster/MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UBTTask_UpdateNextTargetLocation::UBTTask_UpdateNextTargetLocation()
{
  NodeName = TEXT("Stop or starts the movement");
}


EBTNodeResult::Type UBTTask_UpdateNextTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    /* Gets the Monster AI controller, fail the task if it's not present */
	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (MonsterController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

  /* Updates the value of bCanMove in MonsterAI and also updates the Blackboard value */
  MonsterController->StartMovingToNextTargetLocation(CurrnetTargetIndex, bStartOrStop);

  return EBTNodeResult::Succeeded;

}
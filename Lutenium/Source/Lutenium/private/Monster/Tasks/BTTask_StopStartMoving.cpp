
#include "Monster/Tasks/BTTask_StopStartMoving.h"
#include "Monster/MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UBTTask_StopStartMoving::UBTTask_StopStartMoving()
{
  NodeName = TEXT("Stop or starts the movement");
}


EBTNodeResult::Type UBTTask_StopStartMoving::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    /* Gets the Monster AI controller, fail the task if it's not present */
	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (MonsterController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

  /* Updates the value of bCanMove in MonsterAI and also updates the Blackboard value */
  MonsterController->StartOrStopMoving(bStartOrStop);

  return EBTNodeResult::Succeeded;

}
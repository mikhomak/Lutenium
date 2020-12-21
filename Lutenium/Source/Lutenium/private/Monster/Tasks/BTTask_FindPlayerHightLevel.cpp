
#include "Monster/Tasks/BTTask_FindPlayerHightLevel.h"
#include "Monster/MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UBTTask_FindPlayerHightLevel::UBTTask_FindPlayerHightLevel()
{
  NodeName = TEXT("Set the player hight location in blackboard");
}


EBTNodeResult::Type UBTTask_FindPlayerHightLevel::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    /* Gets the Monster AI controller, fail the task if it's not present */
	AMonsterAIController* MonsterController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (MonsterController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

  /* Setting blackboard value of the hight level position of the player in  */
  MonsterController->SetPlayerHightLevelBlackboardValue();

  return EBTNodeResult::Succeeded;

}
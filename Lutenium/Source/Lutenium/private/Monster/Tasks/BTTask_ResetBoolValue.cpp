#include "Monster/Tasks/BTTask_ResetBoolValue.h"
#include "Monster/MonsterAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UBTTask_ResetBoolValue::UBTTask_ResetBoolValue()
{
  NodeName = TEXT("Sets the Value for Bool blackboard key");
}


EBTNodeResult::Type UBTTask_ResetBoolValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

  UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
  if(MyBlackboard == nullptr)
  {
		return EBTNodeResult::Failed;
  }

  MyBlackboard->SetValueAsBool(BoolBlackboardKey.SelectedKeyName, Value);

  return EBTNodeResult::Succeeded;

}
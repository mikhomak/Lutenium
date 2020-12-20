#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindPlayerHightLevel.generated.h"

/**
* Blackboard Task - Setting the hight level of the player in blackboard
*/
UCLASS()
class LUTENIUM_API UBTTask_FindPlayerHightLevel : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:

    UBTTask_FindPlayerHightLevel();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
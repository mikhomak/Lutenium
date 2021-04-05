#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_StopStartMoving.generated.h"

/**
* Blackboard Task - Starts or stops the movement
* bStartOrStop -> true, start the movement. False, stops the mvoement
*/
UCLASS()
class LUTENIUM_API UBTTask_StopStartMoving : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:

    UBTTask_StopStartMoving();


    UPROPERTY(EditAnywhere)
    bool bStartOrStop;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
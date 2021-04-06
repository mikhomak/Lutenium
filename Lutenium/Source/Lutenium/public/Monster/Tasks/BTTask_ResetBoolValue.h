#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ResetBoolValue.generated.h"

/**
* Blackboard Task - Sets Value for BlackboardKey
* @warning works only with bool
*/
UCLASS()
class LUTENIUM_API UBTTask_ResetBoolValue : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:

    UBTTask_ResetBoolValue();


    UPROPERTY(EditAnywhere)
    bool Value;

    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector BoolBlackboardKey; 

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
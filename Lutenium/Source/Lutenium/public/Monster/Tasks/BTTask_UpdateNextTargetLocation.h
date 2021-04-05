#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UpdateNextTargetLocation.generated.h"

/**
* Blackboard Task - Updates the next target location to move to. Optionally can start the movement
* CurrnetTargetIndex -> sums 1 to get the next one
* bStartOrStop -> true, start the movement. False, stops the movemnt
* Uses AMonsterAIController::StartMovingToNextTargetLocation();
*/
UCLASS()
class LUTENIUM_API UBTTask_UpdateNextTargetLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:

    UBTTask_UpdateNextTargetLocation();


    UPROPERTY(EditAnywhere)
    bool bStartOrStop;

    UPROPERTY(EditAnywhere)
    FBlackboardKeySelector CurrnetTargetIndexBlackboardKey; 

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
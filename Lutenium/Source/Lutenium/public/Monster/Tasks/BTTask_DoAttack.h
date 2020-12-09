#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Monster/Weapons/MonsterWeaponType.h"
#include "BTTask_DoAttack.generated.h"

/**
* Blackboard Task - Do attack for a specific weapon, checking if the weapon is present
*/
UCLASS()
class LUTENIUM_API UBTTask_DoAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly)
    EMonsterWeaponType WeaponType;

};
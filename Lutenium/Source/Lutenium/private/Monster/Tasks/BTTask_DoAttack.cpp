
#include "Monster/Tasks/BTTask_DoAttack.h"
#include "Monster/MonsterAIController.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UBTTask_DoAttack::UBTTask_DoAttack()
{
    NodeName = TEXT("Do attack with the weapon");
}


EBTNodeResult::Type UBTTask_DoAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    /* Gets the Monster AI controller, fail the task if it's not present */
    AMonsterAIController* MonsterController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
    if (MonsterController == nullptr)
    {
      return EBTNodeResult::Failed;
    }

    /* Gets weapon from monster with the WeaponType, fails the task if the weapon is null (probably got destroyed) */
    AMonsterWeapon* Weapon = MonsterController->GetWeapon(WeaponType);
    if(Weapon == nullptr)
    {
      return EBTNodeResult::Failed;
    }

    Weapon->DoAttack();

    return EBTNodeResult::Succeeded;

}
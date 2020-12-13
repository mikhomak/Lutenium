
#include "Monster/Decorators/BTDecorator_HasWeapon.h"
#include "Monster/MonsterAIController.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_HasWeapon::UBTDecorator_HasWeapon()
{
    NodeName = TEXT("Has Weapon");
}

bool UBTDecorator_HasWeapon::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    /* Checks if the owner is a type of AMonsterAIController*/
    AMonsterAIController* MonsterController = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
    if (MonsterController == nullptr)
    {
        return false;
    }

    /* Gets weapon from monster with the WeaponType, returns false if it's not available */
    AMonsterWeapon* Weapon = MonsterController->GetWeapon(WeaponType);
    if(Weapon == nullptr)
    {
        return false;
    }

    return true;
}
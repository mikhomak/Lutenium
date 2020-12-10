#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_HasWeapon.generated.h"


UCLASS()
class LUTENIUM_API UBTDecorator_HasWeapon : public UBTDecorator
{
	GENERATED_BODY()


public:

    UBTDecorator_HasWeapon();

    UPROPERTY( EditAnywhere)
    EMonsterWeaponType WeaponType;

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory) const override;
};
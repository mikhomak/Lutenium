
#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/WeaponsUtils/FenceTower2DArray.h"
#include "GameFramework/Actor.h"
#include "PowerSystemMW.generated.h"


UCLASS()
class LUTENIUM_API APowerSystemMW : public AMonsterWeapon
{
	GENERATED_BODY()
public:

    APowerSystemMW();

    // ------------------------------------------------------------------
	// Overrides of AMonsterWeaapon
	// ------------------------------------------------------------------

    /** Shoots projectiles */
    virtual void ExecuteAttack() override;

    // ------------------------------------------------------------------
	// Fence Towers
	// ------------------------------------------------------------------
};
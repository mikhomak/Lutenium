

#include "Monster/Weapons/PowerSystemMW.h"
#include "Monster/Weapons/FenceTowerMW.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Monster/Weapons/WeaponsUtils/FenceTower2DArray.h"


APowerSystemMW::APowerSystemMW() : AMonsterWeapon()
{
    Health = 100.f;
    WeaponType = EMonsterWeaponType::PowerSystem;
}

void APowerSystemMW::ExecuteAttack()
{

}


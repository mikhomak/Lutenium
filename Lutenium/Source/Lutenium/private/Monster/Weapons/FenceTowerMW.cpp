

#include "Monster/Weapons/FenceTowerMW.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"


AFenceTowerMW::AFenceTowerMW() : AMonsterWeapon()
{
    Health = 100.f;
    WeaponType = EMonsterWeaponType::FenceTower;
}

void AFenceTowerMW::ExecuteAttack()
{

}

void AFenceTowerMW::Die()
{
    if(bDebugDetach)
    {
        return;
    }
}


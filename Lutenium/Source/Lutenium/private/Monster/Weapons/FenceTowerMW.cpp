

#include "Monster/Weapons/FenceTowerMW.h"
#include "Monster/Weapons/PowerSystemMW.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Monster/Weapons/WeaponsUtils/FenceTower2DArray.h"


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

    FFenceTower2DArray& FenceTowers = PowerSystem->FenceTowers[HightIndex];
    if(FenceTowers.Num() != 0)
    {
        if(FenceTowers.IsValidIndex(PositionIndex))
        {
            /* Removes the tower from the array without shrinking it!! */
            /* We don't shrink it so we can acces other towers directly by their position index */
            FenceTowers.RemoveAt(PositionIndex, 1, false);
        }
    }

}


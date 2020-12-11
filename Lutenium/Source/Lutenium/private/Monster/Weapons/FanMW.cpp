#include "Monster/Weapons/FanMW.h"
#include "Monster/Weapons/Emp.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Monster/Weapons/MonsterWeaponType.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

AFanMW::AFanMW() : AMonsterWeapon()
{
    Health = 100.f;

    WeaponType = EMonsterWeaponType::Fan;
}

void AFanMW::ExecuteAttack()
{
    if (EmpClass)
    {
        UWorld *World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = MonsterPawn;
            SpawnParams.Instigator = MonsterPawn;
            AEmp *Emp = World->SpawnActor<AEmp>(EmpClass, WeaponMesh->GetSocketLocation(EmpSocket),
                                                WeaponMesh->GetSocketRotation(EmpSocket), SpawnParams);
        }
    }
}

void AFanMW::DoShoot()
{
}
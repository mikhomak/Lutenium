// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Weapons/PipeMW.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"


APipeMW::APipeMW() : AMonsterWeapon()
{
    Health = 100.f;
    WeaponType = EMonsterWeaponType::Pipe;
}

void APipeMW::ExecuteAttack()
{

}

void APipeMW::BeforeAttackEvent_Implementation()
{
    
}
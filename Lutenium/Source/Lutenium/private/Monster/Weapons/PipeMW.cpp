// Fill out your copyright notice in the Description page of Project Settings.


#include "../../../public/Monster/Weapons/PipeMW.h"
#include "../../../public/Monster/Weapons/MonsterWeaponType.h"


APipeMW::APipeMW() : AMonsterWeapon()
{
    Health = 100.f;
    WeaponType = EMonsterWeaponType::Pipe;
}

void APipeMW::DoPipe()
{

}


// Fill out your copyright notice in the Description page of Project Settings.

#include "../../../public/Monster/Weapons/SputnikMW.h"
#include "../../../public/Monster/Weapons/Emp.h"
#include "../../../public/Monster/EnemyMonsterPawn.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

ASputnikMW::ASputnikMW() : AMonsterWeapon()
{
    Health = 100.f;
}

void ASputnikMW::DoEmp()
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

void ASputnikMW::DoShoot()
{
}
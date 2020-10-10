// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterWeapon.h"
#include "SputnikMW.generated.h"

/**
 * 
 */
UCLASS()
class LUTENIUM_API ASputnikMW : public AMonsterWeapon
{
    GENERATED_BODY()
public:

    ASputnikMW();
    
    void Do_Emp();
};

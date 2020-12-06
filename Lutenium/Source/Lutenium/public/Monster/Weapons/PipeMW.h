// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterWeapon.h"
#include "PipeMW.generated.h"

/**
 *
 */
UCLASS()
class LUTENIUM_API APipeMW : public AMonsterWeapon
{
    GENERATED_BODY()
public:

    APipeMW();

    UFUNCTION(BlueprintCallable)
    virtual void ExecuteAttack() override;

};

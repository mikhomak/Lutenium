// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterWeapon.h"
#include "FanMW.generated.h"

/**
 *
 */
UCLASS()
class LUTENIUM_API AFanMW : public AMonsterWeapon
{
    GENERATED_BODY()
public:

    AFanMW();

    virtual void ExecuteAttack() override;

    UFUNCTION(BlueprintCallable)
    void DoShoot();

    UPROPERTY(EditDefaultsOnly, Category = "Emp")
	TSubclassOf<class AEmp> EmpClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = "Emp")
    FName EmpSocket;

};

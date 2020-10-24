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

    UFUNCTION(BlueprintCallable)
    void DoEmp();

    UFUNCTION(BlueprintCallable)
    void DoShoot();

    UPROPERTY(EditDefaultsOnly, Category = "Emp")
	TSubclassOf<class AEmp> EmpClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,  Category = "Emp")
    FName EmpSocket;

};

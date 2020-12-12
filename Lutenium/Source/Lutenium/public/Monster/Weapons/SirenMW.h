// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "SirenMW.generated.h"

/**
 *
 */
UCLASS()
class LUTENIUM_API ASirenMW : public AMonsterWeapon
{
    GENERATED_BODY()
public:

    ASirenMW();


    virtual void ExecuteAttack() override;

    UPROPERTY(EditDefaultsOnly, Category = "Siren")
	TSubclassOf<class AScream> ScreamClass;

    UPROPERTY(EditDefaultsOnly, Category="Siren trigger")
    float SirenForce;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Siren")
    bool bDragOrImpulse;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* SirenTrigger;

    UFUNCTION(BlueprintCallable)
    void SirenTriggerOverlap(class AActor* OtherActor);

    UFUNCTION(BlueprintCallable)
    void SirenTriggerOverlapEnd(class AActor* OtherActor);
};

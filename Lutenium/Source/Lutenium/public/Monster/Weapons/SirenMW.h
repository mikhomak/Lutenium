// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterWeapon.h"
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

    UFUNCTION(BlueprintCallable)
    void DoSiren();

    UPROPERTY(EditDefaultsOnly, Category = "Siren")
	TSubclassOf<class AScream> ScreamClass;

    UPROPERTY(EditDefaultsOnly, Category="Siren trigger")
    float SirenForce;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* SirenTrigger;

    UFUNCTION(BlueprintCallable)
    void SirenTriggerOverlap(
        class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable)
    void SirenTriggerOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

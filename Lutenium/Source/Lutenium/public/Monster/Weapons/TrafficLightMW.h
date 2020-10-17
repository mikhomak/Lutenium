// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterWeapon.h"
#include "TrafficLightMW.generated.h"

/**
 *
 */
UCLASS()
class LUTENIUM_API ATrafficLightMW : public AMonsterWeapon
{
    GENERATED_BODY()
public:

    ATrafficLightMW();

    UFUNCTION(BlueprintCallable)
    void DoRedLight();

    UFUNCTION(BlueprintCallable)
    void DoYellowLight();

    UFUNCTION(BlueprintCallable)
    void DoGreenLight();


    // ------------------------------------------------------------------
    // Lights overlaps
    // ------------------------------------------------------------------

    UFUNCTION(BlueprintCallable)
    void RedTriggerOverlap(
        class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable)
    void RedTriggerOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable)
    void YellowTriggerOverlap(
        class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable)
    void YellowTriggerOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable)
    void GreenTriggerOverlap(
        class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable)
    void GreenTriggerOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

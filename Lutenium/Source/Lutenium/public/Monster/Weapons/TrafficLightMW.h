// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrafficLight.h"
#include "TrafficLightPosition.h"
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

    // ------------------------------------------------------------------
    // Change light
    // ------------------------------------------------------------------

    UFUNCTION(BlueprintCallable)
    void ChangeLight(ETrafficLightPosition Position, ETrafficLight Light);

	UFUNCTION(BlueprintImplementableEvent, Category="Light")
	void OnLightChanged(const ETrafficLightPosition Position, const ETrafficLight Light);

    // ------------------------------------------------------------------
    // Lights status
    // ------------------------------------------------------------------

    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLight LeftLightStatus;

    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLight RightLightStatus;

    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLight CenterLightStatus;


protected:
    // ------------------------------------------------------------------
    // General
    // ------------------------------------------------------------------
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Impact")
    float MissileThrowForce;


    // ------------------------------------------------------------------
    // Lights overlaps
    // ------------------------------------------------------------------
    UFUNCTION(BlueprintCallable)
    void LightBeginOverlap(class AActor* Actor, const ETrafficLight TrafficLightStatus, const ETrafficLightPosition TrafficLightPosition);

    // ------------------------------------------------------------------
    // Lights Meshes
    // ------------------------------------------------------------------

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* LeftLightMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* RightLightMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* CenterLightMesh;

};

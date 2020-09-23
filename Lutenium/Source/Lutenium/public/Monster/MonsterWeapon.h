// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MonsterWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UMonsterWeapon : public USceneComponent
{
    GENERATED_BODY()

public:
    UMonsterWeapon();

    UPROPERTY(VisibleDefaultsOnly, Category=Monster)
    class AEnemyMonsterPawn* MonsterPawn;

    UPROPERTY(VisibleDefaultsOnly, Category=Monster)
    class USkeletalMeshComponent* MonsterMesh;


    // ------------------------------------------------------------------
    // Siren
    // ------------------------------------------------------------------

    UPROPERTY(EditDefaultsOnly, Category=Siren)
    TSubclassOf<class AScream> SirenClass;

    UPROPERTY(EditDefaultsOnly, Category=Siren)
    FName SirenSocketName;

    UFUNCTION(BlueprintCallable, Category=Damage)
    void DoSiren() const;
    
protected:
    virtual void BeginPlay() override;


public:
    FORCEINLINE void SetMonsterMesh(class USkeletalMeshComponent* Mesh) { MonsterMesh = Mesh; }
    FORCEINLINE void SetMonsterPawn(class AEnemyMonsterPawn* Pawn) { MonsterPawn = Pawn; }
};

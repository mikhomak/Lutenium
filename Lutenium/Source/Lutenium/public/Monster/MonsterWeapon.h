// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

#include "MonsterWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API AMonsterWeapon : public AActor
{
    GENERATED_BODY()

public:
    AMonsterWeapon();

    UPROPERTY(VisibleDefaultsOnly, Category=Monster)
    class AEnemyMonsterPawn* MonsterPawn;

    UPROPERTY(VisibleDefaultsOnly, Category=Monster)
    class USkeletalMeshComponent* MonsterMesh;

    UPROPERTY(VisibleDefaultsOnly, Category=Monster)
    class UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleDefaultsOnly, Category=Monster)
    class USphereComponent* Hurtbox;


    UPROPERTY(VisibleDefaultsOnly, Category=Health)
    float Health;

    UFUNCTION()
    virtual void OnTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                      AActor* DamageCauser);
protected:
    virtual void BeginPlay() override;


public:
    FORCEINLINE void SetMonsterMesh(class USkeletalMeshComponent* Mesh) { MonsterMesh = Mesh; }
    FORCEINLINE void SetMonsterPawn(class AEnemyMonsterPawn* Pawn) { MonsterPawn = Pawn; }
};

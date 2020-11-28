// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/MonsterWeaponType.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

#include "MonsterWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API AMonsterWeapon : public AActor
{
    GENERATED_BODY()

public:
    AMonsterWeapon();

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=Monster)
    class AEnemyMonsterPawn* MonsterPawn;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Monster)
    class USkeletalMeshComponent* MonsterMesh;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Monster)
    class UStaticMeshComponent* WeaponMesh;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Monster)
    class USphereComponent* Hurtbox;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Debug")
    EMonsterWeaponType WeaponType;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Debug")
    bool bDebugDetach;


protected:
    virtual void BeginPlay() override;

	// ------------------------------------------------------------------
	// Health & Death
	// ------------------------------------------------------------------
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Health")
    float Health;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Heath")
    float MassInKgAfterDetach;

    UFUNCTION(BlueprintCallable)
    void OnTakeDamage(float Damage);

    UFUNCTION(BlueprintCallable)
    void Die();

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
    void DieEvent();

    /* Take damage & handle death */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;


    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Upgrade")
    int LevelUpgrade;

public:
    FORCEINLINE void SetMonsterMesh(class USkeletalMeshComponent* Mesh) { MonsterMesh = Mesh; }
    FORCEINLINE void SetMonsterPawn(class AEnemyMonsterPawn* Pawn) { MonsterPawn = Pawn; }
    FORCEINLINE void UpgradeWeapon() { LevelUpgrade++; }
    FORCEINLINE void SetUpgradeWeapon(int Level) { LevelUpgrade = Level; }
};

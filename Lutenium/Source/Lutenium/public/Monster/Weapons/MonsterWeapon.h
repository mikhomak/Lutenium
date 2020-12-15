// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
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
    virtual void Die();

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
    void DieEvent();

    /* Take damage & handle death */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;


	// ------------------------------------------------------------------
	// ATTACK
	// ------------------------------------------------------------------

public:
    /* Main action method */
    /* This method has an execution time so the player will know which attack is coming*/
    /* First calls the BeforeAttackEvent to trigger the telegraphics of the attack(sonds, vfx, so on...)*/
    /* After a delay calls the main function of the weapon(some weapons could have no action function)*/
    UFUNCTION(BlueprintCallable, Category="Atack")
    void DoAttack();

protected:
    /* Main method for executing the attack */
    /* Being executes after a certeain delay*/
    UFUNCTION(BlueprintCallable, Category="Atack")
    virtual void ExecuteAttack();

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Atack")
    float TimeBeforeAttack;

	/* Add SVF, VFX and so on the BP to make the attack more telegraphic */
    UFUNCTION(BlueprintImplementableEvent, Category = "Attack")
    void BeforeAttackEvent();

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Upgrade")
    int LevelUpgrade;

    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade")
    void OnUpgradeEvent();


public:
    FORCEINLINE void SetMonsterMesh(class USkeletalMeshComponent* Mesh) { MonsterMesh = Mesh; }
    FORCEINLINE void SetMonsterPawn(class AEnemyMonsterPawn* Pawn) { MonsterPawn = Pawn; }
    FORCEINLINE void UpgradeWeapon() { LevelUpgrade++; OnUpgradeEvent(); }
    FORCEINLINE void SetUpgradeWeapon(int32 Level) { LevelUpgrade = Level; if(Level > LevelUpgrade) { OnUpgradeEvent(); } }
    FORCEINLINE int32 GetUpgradeWeapon() { return LevelUpgrade; }
};

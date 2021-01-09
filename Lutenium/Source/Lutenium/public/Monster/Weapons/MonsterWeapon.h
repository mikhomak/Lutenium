// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

#include "MonsterWeapon.generated.h"

/**
 * Abstract class of the monster weapon
 * Each weapon should extend this class
 * Main method is DoAttack() which sets timer for invoking ExecuteAttack()
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API AMonsterWeapon : public AActor
{
    GENERATED_BODY()

public:
    AMonsterWeapon();
    /* Main weapon Mesh. Root Component */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Weapon")
    class UStaticMeshComponent* WeaponMesh;

    /**
     * Hurtbox collision component
     * You can add more collision in bp
     * Set collision profile name as MonsterWPHurtbox for new collisions
     * Connect it to OnTakeDamage() method to well take damage duh
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Weapon")
    class USphereComponent* Hurtbox;

    /** Weapon type */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Weapon")
    EMonsterWeaponType WeaponType;

	// ------------------------------------------------------------------
	// Monster references
	// ------------------------------------------------------------------

    /* Reference to the monster pawn */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category=Monster)
    class AEnemyMonsterPawn* MonsterPawn;

    /* Reference to the monster skeletal mesh */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Monster)
    class USkeletalMeshComponent* MonsterMesh;

	// ------------------------------------------------------------------
	// Debug
	// ------------------------------------------------------------------

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Debug")
    bool bDebugDetach;

protected:
	// ------------------------------------------------------------------
	// Health & Death
	// ------------------------------------------------------------------

    /** Health of the weapon */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Health")
    float Health;

    /**
     * Mass in kg after detach(death)
     * After diying the weapon got detached and starts simulating physics
     * Put some cool values here so it would fall cool
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Heath")
    float MassInKgAfterDetach;

    /**
     * Doing damage stuff
     * Invoking diying if Health < 0
     * Dealing damage to the monster(transparent damage way weapon->monster)
     * Being called in TakeDamage
     */
    UFUNCTION(BlueprintCallable)
    void OnTakeDamage(float Damage);

    /**
     * Die when health becomes below 0
     * Do not destroyes the weapon
     * Distach it from the weapon and activates physics so it would fall nice and smooth just like i like it
     */
    UFUNCTION(BlueprintCallable)
    virtual void Die();

    /**
     * Do some vfx bullshit in bp
     */
	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
    void DieEvent();

    /* Take damage & handle death */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

public:
	// ------------------------------------------------------------------
	// ATTACK
	// ------------------------------------------------------------------

    /**
     * Main action method
     * This method has an execution time so the player will know which attack is coming
     * First calls the BeforeAttackEvent to trigger the telegraphics of the attack(sonds, vfx, so on...)
     * After a delay calls the main function of the weapon(some weapons could have no action function)
     * @warning This method should be called in TASKS
     */
    UFUNCTION(BlueprintCallable, Category="Atack")
    void DoAttack();

    /*
     * Main method for executing the attack
     * Being executes after a certeain delay
     * @warning override this method in every weapon
     */
    UFUNCTION(BlueprintCallable, Category="Atack")
    virtual void ExecuteAttack();

    /**
     * Time to invoke ExecuteAttack() after invoking DoAttack()
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Atack")
    float TimeBeforeAttack;

    /** Cooldown timer */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Atack")
    float CooldownTime;

    /**
     * Bool to check if the attack can be performed
     * Gets activated in CooldownEnd()
     * Gets deactivated in DoAttack()
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Atack")
    bool bCanAttack;

    FORCEINLINE UFUNCTION(BlueprintCallable, Category="Atack")
    void CooldownEnd()
    {
        bCanAttack = true;
    }

	/* Add SVF, VFX and so on the BP to make the attack more telegraphic */
    UFUNCTION(BlueprintNativeEvent , Category = "Attack")
    void BeforeAttackEvent();

    /** The implementation of the event. Override this in every new weapon! Or else DoAttack() wouldn't work and it would crash the game YIKES*/
    virtual void BeforeAttackEvent_Implementation();

    /* Level of the weapon */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Upgrade")
    int32 LevelUpgrade;

    /* Event is called on upgrade the weapon. Add SVX, VFX and so on in the BP.  */
    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade")
    void OnUpgradeEvent();

public:
    FORCEINLINE void SetMonsterMesh(class USkeletalMeshComponent* Mesh) { MonsterMesh = Mesh; }
    FORCEINLINE void SetMonsterPawn(class AEnemyMonsterPawn* Pawn) { MonsterPawn = Pawn; }
    FORCEINLINE void UpgradeWeapon() { LevelUpgrade++; OnUpgradeEvent(); }
    FORCEINLINE void SetUpgradeWeapon(int32 Level) { LevelUpgrade = Level; if(Level > LevelUpgrade) { OnUpgradeEvent(); } }
    FORCEINLINE int32 GetUpgradeWeapon() { return LevelUpgrade; }
};

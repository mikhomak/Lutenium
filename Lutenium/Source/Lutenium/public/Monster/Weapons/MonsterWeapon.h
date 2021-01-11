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
 * ExecuteAttack is specific for each weapon and does an actual attack
 * Each weapon has a cooldown
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

	// ------------------------------------------------------------------
	// Health & Death
	// ------------------------------------------------------------------

    /** Health of the weapon */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Health")
    float Health;

    /**
     * Doing damage stuff
     * Invoking diying if Health < 0
     * Dealing damage to the monster(transparent damage way weapon->monster)
     * Being called in TakeDamage
     */
    UFUNCTION(BlueprintCallable,Category="Health")
    void OnTakeDamage(float Damage);

    /**
     * Takes damage when missile collides with mesh
     * Redcues damage applied to the weapon
     * Calls OnTakeDamage() with reduced damage
     */
    UFUNCTION(BlueprintCallable,Category="Health")
	void TakeMeshDamage(float Damage);

    /**
     * Apply damage without reduction when the missile hits directly hurtbox
     * Calls OnTakeDamage()
     */
    UFUNCTION(BlueprintCallable, Category="Health")
    void TakeHurtboxDamage(float Damage);

    /**
     * Event when the weapon takes damage
     * We can't use TakeDamage because we have other checks when the weapon recieve damage but it's not applied to the weapon
     * The final method to recieve damage is always OnTakeDamage()
     * Add VFX, SFX all of the shiete
     */
    UFUNCTION(BlueprintImplementableEvent, Category="Health")
    void TakeDamageEvent(float& Damage);

    /**
     * Apply reduced damage when the missile hit the mesh and not the hurtbox
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Health")
    float MeshDamageReduction;

    /**
     * Overriding reciving radial damage
     * Checks if the hit component was Mesh or Hurtbox
     * Calls TakeMeshDamage() or TakeHurtboxDamage()
     */
    virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

    /**
     * Time that's weapon being invincible after taking damage
     * Prevents insta-kills
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Health")
    float InvincibilityTime;

    /**
     * Ends the invincibility and thos weapon can be damaged
     * Fires this method by timer in OnTakeDamage()
     */
    FORCEINLINE UFUNCTION(BlueprintCallable, Category="Health")
    void InvincibilityEnd()
    {
        /* OOTB method lol there is a bool bCanBeDamaged in AActor*/
        SetCanBeDamaged(true);
    }

    /**
     * Die when health becomes below 0
     * Do not destroyes the weapon
     * Distach it from the weapon and activates physics so it would fall nice and smooth just like i like it
     */
    UFUNCTION(BlueprintCallable)
    virtual void Die();

    /**
     * Do some vfx bullshit in bp on death
     */
	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
    void DieEvent();

    /**
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;
     * After diying the weapon got detached and starts simulating physics
     * Put some cool values here so it would fall cool
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Heath")
    float MassInKgAfterDetach;

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
     * Some weapons can repeat the ExecuteAttack()
     * This variable keeps track of the amount of repeated attacks so we can stop repeating it in ExecuteAttack()
     * Resets this value in DoAttack(), which is called in Tasks
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Atack")
    int32 CurrentExecutedAttacks;

    /**
     * When the weapon wants to repeat the attack that variable determines the time between attack
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Atack")
    float RepeatAttackTime;

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

    /**
     * Activates weapon after the cooldown
     * Gets called in DoAttack()
     */
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

    /**
     * Added specific upgrade for each method if it's being needed
     * Virtual method, could be overrated
     * Being called in SetUpgradeWeapon
     */
    virtual void SpecificUpgrade(int32 Level);

    /* Event is called on upgrade the weapon. Add SVX, VFX and so on in the BP.  */
    UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade")
    void OnUpgradeEvent();

public:
    FORCEINLINE void SetMonsterMesh(class USkeletalMeshComponent* Mesh) { MonsterMesh = Mesh; }
    FORCEINLINE void SetMonsterPawn(class AEnemyMonsterPawn* Pawn) { MonsterPawn = Pawn; }
    FORCEINLINE void UpgradeWeapon() { LevelUpgrade++; OnUpgradeEvent(); }
    FORCEINLINE void SetUpgradeWeapon(int32 Level) { LevelUpgrade = Level; if(Level > LevelUpgrade) { OnUpgradeEvent(); SpecificUpgrade(Level); } }
    FORCEINLINE int32 GetUpgradeWeapon() { return LevelUpgrade; }
};

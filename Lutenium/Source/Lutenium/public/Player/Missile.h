
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Missile.generated.h"

/**
 * Normal missile that the player can shoot from the ?gecko?
 * Each tick the players is looking for a hurtbox, if they didn't find one then it will look for the monster, then just fire straight without following the target
 * Priority = MonsterHurtbox > Monster > Nothing(not following/homing)
 * The missile can be thrown away (ThrowMissile()), which disables the movemnt of the missile, and applyes impules force to it
 * When the missile is thrown away it can be Defected - ActivateDefected(). That means it will gravitate the players towards it and eventually blow up pushing them away
 * Can be emped - EmpMissile(). That means it will stop moving and start rotating and falling down
 */
UCLASS()
class LUTENIUM_API AMissile : public AActor
{
	GENERATED_BODY()

public:
	AMissile();

	// -----------------------------------------------------------------------------------------------------------
	// Components and references
    // -----------------------------------------------------------------------------------------------------------

	/** Main sphere component. Root component */
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Missile")
	class USphereComponent* SphereComponent;

	/** Projectile movement component. Carefull the params of this component(Velocity and such). */
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Movement")
    class UProjectileMovementComponent* ProjectileMovement;

	/**
	 * When the missile is defected, activates this component to pull the player in
	 * Force should be negative for that
	 * After being defected, before blowing up, changes the force to the positive value to push the player away
	 */
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Defected")
	class URadialForceComponent* RadialForceComponent;

	/** Mesh of the missile */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Mesh")
	class UStaticMeshComponent* MissileMesh;


	// -----------------------------------------------------------------------------------------------------------
	// Movement activation
    // -----------------------------------------------------------------------------------------------------------

	/**
	 * Set target for homing attack or the direction if the Target is nullptr
	 * Being called from the PlayerPawn
	 * @param Target - target to follow or nullptr. Target usually is MonsterWeapon or the Monster itself
	 * @param ShootDirection - if the Target is nullptr, the missile will fly to this directon. Should be normalized
	 */
	UFUNCTION(BlueprintCallable)
	void SetTargetOrDirection(class USceneComponent* Target, const FVector& ShootDirection);

	/**
	 * Throw missle in a specified direction
	 * Disables movement(homing or going straight)
	 * Can defect missile right away
	 * @param ThrownDirection - Direction to throw(Should be normalized duh)
	 * @param ForceAmount - amount of the impulse
	 * @param bDefectMissile - specifies if the missile should be defected or nah
	 */
	UFUNCTION(BlueprintCallable)
	void ThrowMissile(FVector ThrownDirection, float ForceAmount, bool bDefectMissile);

	/**
	 * Activates emp for the missile
	 * Disables current movement and activates gravity, while random rotating the missile
	 */
	UFUNCTION(BlueprintCallable)
	void EmpMissile();

	// -----------------------------------------------------------------------------------------------------------
	// Defected activation
    // -----------------------------------------------------------------------------------------------------------

	/**
	 * When the missile gets defected, call this method
	 * It will activate RadialForceComponent
	 */
	UFUNCTION(BlueprintCallable, Category = "Defected")
	void DefectMissile();


protected:

	// -----------------------------------------------------------------------------------------------------------
	// Speed, health n general
    // -----------------------------------------------------------------------------------------------------------

	/** Life span of the missile, if it doesn't hit somteting, just blows up after som amount of time */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="General")
	float MissileLifeSpan;

	/**
	 * When the missile doesn't have a target (SetTargetOrDirection()), determines the speed of the missile
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="General")
	float NonTargetSpeed;


	// -----------------------------------------------------------------------------------------------------------
	// Damage
    // -----------------------------------------------------------------------------------------------------------

	/** Amount of radial damage that will be applied on explosing */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Damage")
	float Damage;

	/** Radial damage radius on explosing */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Damage")
	float ExplosionRadius;

	/**
	 * When the missile hits something, we should call this method!
	 * Also at destroy
	 * Invokes OnExplode event and creates radial damage
	 */
	UFUNCTION(BlueprintCallable, Category="Damage")
	void Explode();

	/**
	 * Event that get invoked before the applying radial damage on the explosion
	 * If the missile is defectes, use OnDefectedImpulse()
	 * Add some VFX, SFX and other cool stuff in BP
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
	void OnExplode();


	// -----------------------------------------------------------------------------------------------------------
	// Defected
    // -----------------------------------------------------------------------------------------------------------


	/**
	 * How much time should a missile be defected before blowing up?
	 * Removes LifeSpan timer
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Defected")
	float DefectedTime;


	/**
	 * After the DefectedTime timer, calls this function to push the player away from the defected missile
	 */
	UFUNCTION(BlueprintCallable, Category = "Defected")
	void DefectedMissileImpulse();

	/**
	 * When the missile is defected determines how much force we should apply to the RadialForceComponent
	 * @warning must be negative
	 */
	UPROPERTY(EditDefaultsOnly, Category="Defected")
	float DefecteedGravityForceAmount;

	/**
	 * When the DefectedTime timer passes, pushes the player away
	 */
	UPROPERTY(EditDefaultsOnly, Category="Defected")
	float DefecteedImpulseForceAmount;

	/**
	 * When the missile is defected determines the radius of the RadialForceComponent
	 */
	UPROPERTY(EditDefaultsOnly, Category="Defected")
	float DefecteedGravityRadius;

	/**
	 * When the missile is defected determines the radius of the explosion of the RadialForceComponent
	 */
	UPROPERTY(EditDefaultsOnly, Category="Defected")
	float DefecteedImpulseRadius;

	/**
	 * Determines if the player is defected
	 * Getting activated in ActivateDefected() and ThrowMissile()
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Defected")
	bool bIsDefected;

	/**
	 * Event when the the missile just got defected
	 * Add some VFX, SFX and other cool stuff in BP
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Defected")
	void GotDefected();


	/**
	 * Event when the the missile is gonna blow up and it's defected (don't use OnExplode in this case)
	 * Add some VFX, SFX and other cool stuff in BP
	 * Destroy actor(with delay) after you are done
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Defected")
	void OnDefectedImpulse();

	// -----------------------------------------------------------------------------------------------------------
	// Emp
    // -----------------------------------------------------------------------------------------------------------

	/**
	 * Randomly rotate the missile while falling down
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Emp")
	FRotator EmpRotator;

	/**
	 * Amount of rotation force while emp
	 */
	UPROPERTY(EditDefaultsOnly, Category="Emp")
	float EmpRotationForce;

	/**
	 * Gravity scale while falling down with emp applied to ProjectileMovement
	 */
	UPROPERTY(EditDefaultsOnly, Category="Emp")
	float EmpGravityScale;

	/**
	 *  Event when the missile gets emped
	 * Add some VFX, SFX and other cool stuff in BP
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Defected")
	void OnEmped();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};


// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Player/PlayerPlaneMovementComponent.h"
#include "MissileTargetHit.h"
#include "PlayerPawn.generated.h"



/**
 * Available upgrades for the player
 */
UENUM(BlueprintType)
enum class EPlayerUpgrade : uint8
{
	IncreasedAimRadius UMETA(DisplayName = "Increased missile aim radius"),					// Increases aim radiusfor the missiles
	DoubleMissileAimLock UMETA(DisplayName = "Double missile aim lock"),					// Adds second missile aim lock
	BaseSupport UMETA(DisplayName = "Base support missile"),								// Support missile from the base to the current aim lock
	MachineGun UMETA(DisplayName = "Machine Gun"),													// Just a gun
	IncreasedDyingVelocity  UMETA(DisplayName = "Increased velocity to die on hit"),		// Increases the velocity needed to die on hit
	BarrelRoll UMETA(DisplayName = "Fast barrerl roll"),									// DO A BARREL ROLL (fast one)
	BackMirrors UMETA(DisplayName = "Back mirrors") 										// Adds Back mirrors to the UI
};


/**
 * Player weapons
 * Could be switched between
 * Missile by default
 */
UENUM(BlueprintType)
enum class EPlayerWeapon : uint8
{
	Missile UMETA(DisplayName = "Missile"),
	MachineGun UMETA(DisplayName = "MachineGun")
};



/**
 * Main Player class
 * For movement uses - PlayerPlaneMovementComponent  which is PlaneMovementComponent with integrated Abilities for the player
 * Has upgrade system  - EPlayerUpgrade
 * For combat uses - missiles
 * For raycasting uses - FAssistsUtils
 */
UCLASS(Config=Game)
class APlayerPawn : public APawn
{
	GENERATED_BODY()

public:

	/**
	 * Root Component
	 * Main component that moves the player
	 * Physic based
	 * Don't forrget to set size in BP
	 */
	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, Category = "General")
	class UBoxComponent* PlaneBox;

	/**
	 * Main mesh of the jet
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* PlaneMesh;

	/**
	 * Spring arm for the camera
	 * You can change the length of it in BP according to speed
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArm;

	/**
	 * Main camera of the player
	 * Configure it in BP
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* Camera;

	/**
	 * Main movement component
	 * Every tick executes UPlaneMovementComponent::TickComponent() that has main logic of the logic
	 * In order to install it correctly, set OwnerPawn, PhysicsComponent and PlayerPawn of PlaneMovement in the constructor of the PlayerPawn
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	class UPlayerPlaneMovementComponent* PlaneMovement;

	/** Constructor */
	APlayerPawn();

	/**
	 * Tick method
	 * Executes MissileAimLock()
	 * @see MissileAimLock()
	 */
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * On hit rotates the player
	 * TODO decide what to do it with it
	 */
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	/** Returns the Yawn Imput */
	UFUNCTION(BlueprintCallable, Category = "Input")
	float GetYawnInput() const;

	/** Returns the Roll Imput */
	UFUNCTION(BlueprintCallable, Category = "Input")
	float GetRollInput() const;

	/** Returns the Pitch Imput */
	UFUNCTION(BlueprintCallable, Category = "Input")
	float GetPitchInput() const;

	/** Returns the Thrust Imput */
	UFUNCTION(BlueprintCallable, Category = "Input")
	float GetThrustInput() const;

	/**
	 * Event when Dot value has Changed dramatically
	 * Dot value - dot product between current physics velocity and UP vector
	 * When the value has reached a certain point, invokes this event
	 * It was made for activating "drift lines", when aerdonyamcis is activated
	 * Activate drift lines in BP
	 * @see UPlaneMovementComponent - CalculateAerodynamic()
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "PlaneMovement")
	void DotHasChange(const float Dot);

	/**
	 * Event called when Dash has been activated
	 * Do some fancy shit with camera n VFX/SFX in BP
	 * @see UPlayerPlaneMovementComponent - DashInput()
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "PlaneMovement")
	void DashImpact();


	// ------------------------------------------------------------------
	// Weapons
	// ------------------------------------------------------------------

	/**
	 * Current weapon
	 * Could be switch in SwitchWeapons()
	 * Missile - Shoots missiles. Should be pressed everytime to fire them
	 * MachineGun - Shoots bullets. Can be hold
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	EPlayerWeapon CurrentWeapon;

	/**
	 * Fires the weapon
	 * Sets bWeaponFiring = true
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void WeaponInputPressed();

	/**
	 * Sets bWeaponFiring = false
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void WeaponInputReleased();

	/**
	 * Indicates whenever the weapon input is hold or not
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	bool bWeaponFiring;

	/**
	 * Switches the current weapon
	 * Updates CurrentWeapon
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapons")
	void SwitchWeapons(EPlayerWeapon NextWeapon);

	/**
	 * Event when the weapon has changed
	 * Called in SwitchWeapons()
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapons")
	void OnSwitchWeaponEvent(EPlayerWeapon NextWeapon);

	// ------------------------------------------------------------------
	// MachineGun
	// ------------------------------------------------------------------

	/**
	 * Indicates whenever the player has a machine gun
	 * That's an upgrade
	 * Changes in UpgradePlayer()
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|MachineGun")
	bool bHasMachineGun;

	/**
	 * Bullet class of a machine gun to spawn
	 * Maybe I would create a base class in cpp for that, but this bullet should be straightforward and can be done in BP
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons|MachineGun")
	TSubclassOf<class AActor> MachineGunBulletClass;

	/**
	 * Fire rate of a MachineGun when the weapon input is hold (bWeaponFiring == true)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|MachineGun")
	float MachineGunFireRate;

	/**
	 * Timer to handle the FireMachineGun()
	 * We need to save it in order to clear the timer
	 * Starts in WeaponInputPressed()
	 * Ends in WeaponInputReleased()
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|MachineGun")
	FTimerHandle MachingGunTimerHandler;

	/**
	 * Spawns BulletClass each MachineGunFireRate
	 * Invokes in the WeaponInputPressed() with MachingGunTimerHandler
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapons|MachineGun")
	void FireMachineGun();

	/**
	 * Event when the bullet gets fired from a machine gun
	 * Being called in FireMachineGun()
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapons|MachineGun")
	void OnFireMachineGunEvent();

	// ------------------------------------------------------------------
	// Missile
	// ------------------------------------------------------------------

	/**
	 * Each tick we are looking for the target for the missile
	 * It's an array for the possible upgraded missiles
	 * First index (0) is awlays present because it doesn't need an upgrade
	 * Others indexes are use for upgraded missiles (AmountOfFireMissile and bHasDoubleAimLocks)
	 * We use FAssistUtils::RaycastMissileTarget() for the first missile
	 * For the upgaded missiles we use FAssistUtils::RaycastUpgradedMissileTarget()
	 * The possible values could be: MonsterWeapon or Monster itself. nullptr in other cases
	 * If upgraded values can't find new values, they become the same as the [0]
	 * @see MissileAimLock()
	 * @see AmountOfFireMissile
	 * @see bHasDoubleAimLocks
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|Missile")
	TArray<USceneComponent*> MissileTargetArray;

	/**
	 * Array of locations of MissileTargetArray
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|Missile")
	TArray<FVector> MissileTargetRaycastHitLocationArray;

	/**
	 * Hit type of the main missile - MissileTargetArray[0]
	 * If it's EMissileTargetHit::MonsterWPHurtbox then we will raycast the ugrpades ones
	 * If it's EMissileTargetHit::Monster or EMissileTargetHit::NoHit then we will use [0] for the upgrade ones
	 * @see MissileAimLock()
	 * @see MissileTargetArray
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons|Missile")
	EMissileTargetHit MissileTargetRaycastHitType;

	/**
	 * Missicle class to spawn
	 * Create a BP child class of AMissile and assing it to this
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons|Missile")
	TSubclassOf<class AMissile> MissileClass;

	/**
	 * The length to raycast for the missile
	 * Could be upgraded with EPlayerUpgrade::IncreasedAimRadius
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons|Missile")
	float MissileAimTraceLength;

	/**
	 * When we ryacast for the first missile, we raytrace as a sphere with this radius
	 * The first raycast that we do, we are searching for the MonsterWPHurtbox
	 * @see MissileAimLock()
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons|Missile")
	float FirstRaytraceMissileAimRadius;

	/**
	 * When the first raycast failes, search for the Monster with bigger radius
	 * @see FirstRaytraceMissileAimRadius
	 * @see MissileAimLock()
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons|Missile")
	float SecondRaytraceMissileAimRadius;

	/**
	 * When the first missile found MonsterWeapon, then we will raycast the upgrade ones IF we the upgrade is available (bHasDoubleAimLocks == true)
	 * @see FirstRaytraceMissileAimRadius
	 * @see MissileAimLock()
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Weapons|Missile")
	float UpgradeRaytraceMissileAimRadius;

	/**
	 * Main method to fire missiles
	 * Fire AmountOfFireMissile missiles
	 * If the first missile hasn't found anything, then every missile will go staright
	 * If every next missile (from 0 - AmountOfFireMissile) doesn't find anyting, uses the last found position
	 * EXAMPLE - if we have 3 missilse, and found hurtbox on the first one and haven't found one on the second one, then 2-3 will follow [0]
	 * @see MissileAimLock()
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapons|Missile")
	void FireMissile();

	/**
	 * Checks every tick for the possible missile aim lock
	 * First, raycasting sphere in front of the player for MonsetrWPHurtbox collision type
	 * If nothing has been found, raycast for Monster collision type
	 * If nothing has been found for the monster either, then just indicates that nothing has been found
	 * Updates :
	 *			1. MissileTargetArray[0] and other values of array depending on the upgrade
	 *			2. MissileTargetRaycastHitLocationArray[0] and other values of array depending on the upgrades
	 *			3. MissileTargetRaycastHitType
	 * If the MissileTargetRaycastHitType == EMissileTargetHit::MonsterWPHurtbox (previously we found hurtbox), then we check if this location (MissileTargetRaycastHitLocationArray[0]) is still available and indeed is Hurtbox
	 * If so, we don't change it. If it is not, then we execute this method as normal
	 *
	 * If the player has aquiered EPlayerUpgrade::DoubleMissileAimLock (bHasDoubleAimLocks == true), then we shoot two missiles
	 * First, we check if there is two hurtbox that we can aim lock. If true then we add that to MissileTargetRaycastHitLocationArray[1] and MissileTargetArray[1]
	 * If not, then MissileTargetRaycastHitLocationArray[1] and MissileTargetArray[1] will have the same values as the first one
	 * @warning this method does not fire missiles, it just updates the Aim Lock variables. To fire missile use FireMissile()
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapons|Missile")
	void MissileAimLock();

	/**
	 * Indicates the amount of missiles that will be fired in FireMissile()
	 * Each missile need its own MissileTargetArray[i] and MissileTargetRaycastHitLocationArray[i]
	 * They could have the sames values
	 * @see MissileAimLock()
	 * @see bHasDoubleAimLocks
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons|Missile")
	int32 AmountOfFireMissile;

private:

	/**
	 * Actors to ignore while raycasting for missile aim lock
	 * The first value is always @this
	 * The size of the array is determined by AmountOfFireMissile
	 * Uses for the upgraded aim lock
	 */
	TSet<AActor*> IgnoredActors;

public:
	// ------------------------------------------------------------------
	// Upgrades
	// ------------------------------------------------------------------

	/**
	 * Gives the player new upgrade
	 * If the upgrade is already aquiered, do nothing
	 * @param NewUpgrade - new upgrade to aquiere
	 */
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	void UpgradePlayer(const EPlayerUpgrade NewUpgrade);

	/**
	 * Checks if the upgrade has been aquiered or not
	 * @param Upgrade - check if this ugrpade has a value of true in UpgradeMap
	 * @return true if it has been aquiered, false it hasn't been aquiered
	 */
	UFUNCTION(BlueprintPure, Category = "Upgrades")
	bool IsUpgradeAquiered(const EPlayerUpgrade Upgrade);

	/**
	 * Map that contains upgrades
	 * Key - Enum of upgrade
	 * Value - bool that indicates if the upgrade has been aquiered
	 * Updates in UpgradePlayer()
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Upgrades")
	TMap<EPlayerUpgrade, bool> UpgradeMap;


	/**
	 * Upgrade for EPlayerUpgrade::IncreasedAimRadius
	 * Increases FirstRaytraceMissileAimRadius, SecondRaytraceMissileAimRadius and MissileAimTraceLength
	 * Basically makes aim lock bigger
	 * @warning - value should be bigger than 1
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Upgrades|Upgrade values", meta = (ClampMin = "1", UIMin = "1"))
	float IncreasedAimRadiusMultiplier;

	/**
	 * Upgrade for EPlayerUpgrade::DoubleMissileAimLock
	 * Adds the second aim lock
	 * If second aim lock hasn't been found, uses two missiles for the first aim lock
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Upgrades|Upgrade values")
	bool bHasDoubleAimLocks;

	/**
	 * Event when the player got an upgrade
	 * Invokes only if the upgrade hasn't been aquiered before
	 * @see UpgradePlayer()
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Upgrade")
	void AquieredUpgrade(EPlayerUpgrade Upgrade);

	// ------------------------------------------------------------------
	// Other Events
	// ------------------------------------------------------------------

	/**
	 * Effects when emp gets activated
	 * @see UPlayerEmpMovementEffect
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Effects")
	void OnEmpActivateEvent();

	/**
	 * Effects when emp gets deactivated
	 * @see UPlayerEmpMovementEffect
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Effects")
	void OnEmpDeactivateEvent();

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	UInputComponent* InputComponent;


public:
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE class UPlaneMovementComponent* GetPlaneMovement() const { return PlaneMovement; }
	FORCEINLINE class UBoxComponent* GetPlaneBox() const { return PlaneBox; }
};

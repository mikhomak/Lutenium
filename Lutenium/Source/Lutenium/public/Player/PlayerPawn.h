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
	IncreasedAimRadius UMETA(DisplayNmae= "Increased missile aim radius"),				// Increases aim radiusfor the missiles
	DoubleMissileAimLock UMETA(DisplayNmae= "Double missile aim lock"),					// Adds second missile aim lock
	BaseSupport UMETA(DisplayNmae= "Base support missile"),								// Support missile from the base to the current aim lock
	Gun UMETA(DisplayNmae= "Machine Gun"),												// Just a gun
	IncreasedDyingVelocity  UMETA(DisplayNmae= "Increased velocity to die on hit"),		// Increases the velocity needed to die on hit
	BarrelRoll UMETA(DisplayNmae= "Fast barrerl roll")									// DO A BARREL ROLL (fast one)
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

public:

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
	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	float GetRollInput() const;

	/** Returns the Pitch Imput */
	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	float GetPitchInput() const;

	/** Returns the Thrust Imput */
	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
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
	void DotHasChange();

	/**
	 * Event called when Dash has been activated
	 * @see UPlayerPlaneMovementComponent - DashInput()
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "PlaneMovement")
	void DashImpact();

	// ------------------------------------------------------------------
	// Missile
	// ------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
	TArray<USceneComponent*> MissileTargetArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
	TArray<FVector> MissileTargetRaycastHitLocationArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Missile")
	EMissileTargetHit MissileTargetRaycastHitType;

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
	TSubclassOf<class AMissile> MissileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
	float MissileAimTraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
	float FirstRaytraceMissileAimRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
	float SecondRaytraceMissileAimRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Missile")
	float UpgradeRaytraceMissileAimRadius;


	UFUNCTION(BlueprintCallable, Category = "Missile")
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
	UFUNCTION(BlueprintCallable, Category = "Missile")
	void MissileAimLock();

	/**
	 * Indicates the amount of missiles that will be fired in FireMissile()
	 * Each missile need its own MissileTargetArray[i] and MissileTargetRaycastHitLocationArray[i]
	 * They could have the sames values
	 * @see MissileAimLock()
	 * @see bHasDoubleAimLocks
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Missile")
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
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
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

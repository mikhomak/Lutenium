#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MonsterLeg.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Components/TimelineComponent.h"
#include "EnemyMonsterPawn.generated.h"

/**
 * Main enemy monster
 * Use it as a reference to get components that you need because it doesn't do much
 * All the attack are coming from MonsterWeapons
 * Controlled by MonsterAIController
 * Use it as just the body, all the behaviour should come from BehaviourTree
 * All the logic of how to do should come from MonsterAIController
 * @see MonsterWeapon.h
 * @see MonsterAIController.h
 */
UCLASS()
class LUTENIUM_API AEnemyMonsterPawn : public APawn
{
	GENERATED_BODY()

public:

	AEnemyMonsterPawn();

	virtual void Tick(float DeltaTime) override;

	// ------------------------------------------------------------------
	// General
	// ------------------------------------------------------------------

	/**
	 * Main Sphere component
	 * Root Component
	 * @todo consider placing skeletal mesh as a root component
	 */
	UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly)
	class USphereComponent* SphereComponent;

	/** Main skeleton mesh for the monster */
	UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly)
	class USkeletalMeshComponent* MonsterMesh;

	/**
	 * Movement component
	 * We don't need complex behaviour for the monster
	 * It should simply go from place A to place B without
	 */
	UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly)
	class UFloatingPawnMovement* PawnMovement;

	// ------------------------------------------------------------------
	// Player behaviour
	// ------------------------------------------------------------------

	/**
	 * If the player has entered this sphere, notify the behaviour tree (blackbaord)
	 * Should activate the beam defense in this case
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	class USphereComponent* BeamDefenseSphere;

	/**
	 * Handles overlapping BeamDefenseSphere with the player
	 * Invokes SetIsPlayerInRadiusOfBeamDefense(true) from MonsterAIController
	 * Connect this method for BeamDefenseSphere->OnOverlapBegin in BP
	 */
	UFUNCTION(BlueprintCallable)
	void PlayerHasEnteredBeamDefense(class AActor* OverlapActor);

	/**
	 * Handles overlap exit BeamDefenseSphere with the player
	 * Invokes SetIsPlayerInRadiusOfBeamDefense(true) from MonsterAIController
	 * Connect this method for BeamDefenseSphere->OnOverlapBegin in BP
	 */
	UFUNCTION(BlueprintCallable)
	void PlayerHasExitdBeamDefense(class AActor* OverlapExitActor);

	// ------------------------------------------------------------------
	// Legs
	// ------------------------------------------------------------------

	UFUNCTION(BlueprintCallable)
	FVector GetLegLocation(int32 LegIndex) const;

	UPROPERTY(EditDefaultsOnly, Category="Legs movement")
	float LegDistanceBetweenLegsToMove;

	UPROPERTY(EditDefaultsOnly, Category="Legs movement")
	float LegRaycastDownLength;

	UPROPERTY(EditDefaultsOnly, Category="Legs movement")
	float LegLerpValue;

	UPROPERTY(EditDefaultsOnly, Category="Legs movement")
	float LegAddedHightStep;

	UPROPERTY(EditDefaultsOnly, Category="Legs movement")
	float LegStepTime;

	void LegHasMovedEventCaller(const int32 LegIndex);


	// ------------------------------------------------------------------
	// Weapons
	// ------------------------------------------------------------------

	/**
	 * When the weapon "dies", it calls this method
	 * Removes a wepon from the WeaponMap and upgrades all the other weapons
	 */
	UFUNCTION(BlueprintCallable)
	void LooseWeapon(EMonsterWeaponType WeaponType);

	/**
	 * Each weapon is spawned in BeginPlay() of this pawn
	 * Then it stores the refernce to itself in this array with the key of enum EMonsterWeaponType
	 * To get the weapon you need, use this map
	 * @warning TowerFence is always nullptr, use PowerSystem instead
	 * @warning could be nullptr!!! when the weapon is destroyed
	 * @see MonsterWeapon
	 * @see MonsterWeaponType
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Weapons")
	TMap<EMonsterWeaponType, class AMonsterWeapon*> WeaponMap;

	// ------------------------------------------------------------------
	// AI
	// ------------------------------------------------------------------

	/**
	 * The brain of the mosnter
	 * Makes all the decesion
	 * @warning MAKE LOGIC IN THE BEHAIVOR TREE ONLY
	 */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	/**
	 * The controller of the monster
	 * Try to use controller(in all logic based stuff, ex: tasks, services, etc..) instead of this class itself as much as possible
	 */
	UPROPERTY(VisibleDefaultsOnly, Category = "AI")
	class AMonsterAIController* MonsterAI;

	// ------------------------------------------------------------------
	// DAMAGE & HEALTH
	// ------------------------------------------------------------------

	/**
	 * Health of the monster
	 * Takes damages from the player directly(to the monster mesh) or inderectly(to the weapons/hurtboxes)
	 * Direct damage is reduced
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health")
	float Health;

	/* Percentage value of the damage that is going to be applied */
	/* Should be from 0 to 1 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health", meta = (UIMin = "0.0", UIMax = "1.0"))
	float DirectDamageReduction;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die();

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void OnDieEvent();

	/* Handles damage from the weapon. Do not reduce the damage */
	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeNonDirectDamage(float Damage);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	bool bHandleDeathInCpp;

	/* Handles direct damage from the player. Reduces the actual damage  */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;


	// ------------------------------------------------------------------
	// Other socket locations
	// ------------------------------------------------------------------

	/** Checks on what hight level player is at */
	/** The hight level is determined by the monster hight location and its sockets */
	/** Check mesh to see where the socket are */

	/** Checks if the player is below that socket's location to fire the appropiate weapon*/
	/** First hight level(the lowest one) to decide what weapon should be used for an attack */
	/** RN it's traffic light*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sockets")
	FName FirstHightLevelSocketName;

	/** Checks if the player is below that socket's location to fire the appropiate weapon*/
	/** RN it's fan*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sockets")
	FName SecondHightLevelSocketName;

	/** Checks if the player is ABOVE that socket's location to fire the appropiate weapon*/
	/** RN it's siren */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sockets")
	FName ThirdHightLevelSocketName;

	/** Returns the world location of the hight level from the socket */
	/** Start with 1, ends with 3 */
	UFUNCTION(BlueprintCallable, Category = "Sockets")
	FVector GetHightLevelSocketLocation(const int32 Location);

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

	// ------------------------------------------------------------------
	// Legs
	// ------------------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Legs")
	TArray<class UMonsterLegComponent*> Legs;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
	class UMonsterLegComponent* LeftFrontLeg;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
	class UMonsterLegComponent* LeftMiddleLeg;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
	class UMonsterLegComponent* LeftBackLeg;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
	class UMonsterLegComponent* RightFrontLeg;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
	class UMonsterLegComponent* RightMiddleLeg;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
	class UMonsterLegComponent* RightBackLeg;

	UFUNCTION(BlueprintImplementableEvent)
	void LegHasMoved(EMonsterLeg Leg);

	void InitLeg(class UMonsterLegComponent* Leg, int32 LegIndex);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
	bool bIsCurrentLegsOdd;
	// ------------------------------------------------------------------
	// Body Movement
	// ------------------------------------------------------------------

	UFUNCTION(BlueprintCallable)
	bool RaycastLegJoints();

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	FName BodySocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	FVector BodyPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	float BodyUpMovementSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Body")
	bool bIsBodyMovingUp;

	UFUNCTION()
	void BodyTimelineMovement();

	UFUNCTION()
	void BodyTimelineMovementFinish();

	UPROPERTY(EditDefaultsOnly, Category="Body")
	UCurveFloat* BodyFloatCurve;

	struct FTimeline BodyTimeline;

	FORCEINLINE FVector GetCurrentBodyPosition() const { return BodyPosition; }

	// ------------------------------------------------------------------
	// BODY MOVEMENT WHEN THERE IS AN OVERLAP FROM THE BODY TO THE LEGS
	// ------------------------------------------------------------------

	void ToggleWhatLegsShouldMove(const bool Odd);

	TArray<FName> TopSocketLocationNames;

	bool bBodyMoving;

	void CheckBodyAltitudeDependingOnLegs();


	// ------------------------------------------------------------------
	// Weapons
	// ------------------------------------------------------------------

	UPROPERTY( EditDefaultsOnly,BlueprintReadOnly, Category ="Weapons")
	FName FanSocketName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category ="Weapons")
	FName SirenSocketName;

	UPROPERTY( EditDefaultsOnly,BlueprintReadOnly, Category ="Weapons")
	FName PipeSocketName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category ="Weapons")
	FName TrafficLightSocketName;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category ="Weapons")
	FName PowerSystemSocketName;

	UFUNCTION(BlueprintCallable)
	void SpawnWeapons();


	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<class APipeMW> PipeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<class AFanMW> FanClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<class ASirenMW> SirenClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<class ATrafficLightMW> TrafficLightClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<class APowerSystemMW> PowerSystemClass;
};

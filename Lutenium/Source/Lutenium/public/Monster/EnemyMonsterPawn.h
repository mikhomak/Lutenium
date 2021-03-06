#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/Spells/Scream.h"
#include "GameFramework/Pawn.h"
#include "MonsterLeg.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Components/TimelineComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
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
     * Creating dynamic material 
     * We need it in order to make the flash effect
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Weapon")
    class UMaterialInstanceDynamic* MainMaterialInstance;

	/**
	 * Movement component
	 * We don't need complex behaviour for the monster
	 * It should simply go from place A to place B without complicated bullshiete
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

	UFUNCTION(BlueprintCallable)
	void LegHasMovedEventCaller(const int32 LegIndex, const FVector& Position);


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

	/**
	 * Current Weapon level of each weapons
	 * Upgrades on LooseWeapon() by loosing the weapon
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
	int32 CurrentWeaponLevel;


	/**
	 * Weapon level on which monster start making screams on each step
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Weapons|Step Scream")
	int32 StepScreamWeaponLevelNeeded;

	/**
	 * Spawns scream at each step
	 * Should be BP derived class
	 * Spawns in SpawnStepScream()
	 */
    UPROPERTY(EditDefaultsOnly, Category = "Weapons|Step Scream")
	TSubclassOf<class AScream> StepScreamClass;

protected:

	/**
	 * Spawn scream on the position of the stop movement of the leg
	 * Executes if StepScreamWeaponLevelNeeded >= CurrentWeaponLevel
	 * Invokes in LegHasMovedEventCaller()
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapons|Step Scream")
	void SpawnStepScream(FVector Position);

public:

	// ------------------------------------------------------------------
	// AI
	// ------------------------------------------------------------------

	/**
	 * The brain of the mosnter
	 * Makes all the decesion
	 * @warning MAKE LOGIC IN THE BEHAIVOR TREE ONLY
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	/**
	 * The controller of the monster
	 * Try to use controller(in all logic based stuff, ex: tasks, services, etc..) instead of this class itself as much as possible
	 */
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "AI")
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
	void TakeNonDirectDamage(float Damage, bool bDirectDamage);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	bool bHandleDeathInCpp;

	/* Handles direct damage from the player. Reduces the actual damage  */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;


	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
	void TakeDamageEvent(const float Damage, const bool bDirectDamage);


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
        // Stop flashing
        if(bShouldFlash && MainMaterialInstance != nullptr)
        {
            MainMaterialInstance->SetScalarParameterValue(FlashParameterName, 0.f);
        }
        /* OOTB method lol there is a bool bCanBeDamaged in AActor*/
        SetCanBeDamaged(true);
    }

    /**
     * Should material flash on hit?
     * Flash time is the same of InvincibilityTime
     * Modifes FlashParameterName parameter of MainMaterialInstance
     * RN it's float to lerp between normal texture and flash texture
     * @TODO consider make the separate timer for it?
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health|Flash")
    bool bShouldFlash;

    /**
     * Paramter of MainMaterialInstance to do the flashy flash
     * RN it's float to lerp between normal texture and flash texture
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health|Flash")
    FName FlashParameterName;

	// ------------------------------------------------------------------
	// Movement
	// ------------------------------------------------------------------

	/**
	 * Current direction where the mosnter would go
	 * Uses in PawnMovement->AddInputVector() in each Tick
	 * Calculates in SetDirectionToMove()
	 * Value should be normalized
	 */
	UPROPERTY(BlueprintReadWrite, Category="Movement")
	FVector DirectionToMove;


	/**
	 * Sets direction to move based on the vector in world space
	 * Calculates (PositionWS - ActorLocation()).Normalized
	 * @param PositionWS - position of the target in world space
	 */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetDirectionToMove(FVector& PositionWS);

	/**
	 * Because legs use IK and can pull the body far away from the actor location we need to make sure that the actor will wait untill the mesh comes to its place\
	 * This value determines how far the mesh should for actor to stop
	 * To check this value it uses - Dist(GetActorLocation(), GetCurrentBodyPosition())
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	float fDistanceBetweenBodyMeshAndActorToStop;


	/**
	 * When the actor was waiting for the mesh to catch up, checks the distance when the actor should start moving again
	 * @see fDistanceBetweenBodyMeshAndActorToStop
	 * @see bIsActorWaitingForBody
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement")
	float fDistanceBetweenBodyMeshAndActorToStartMoving;
	
	/**
	 * Indicates if the actor is waiting for the body to catch up
	 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Movement")
	bool bIsActorWaitingForBody;

	/**
	 * Handles actor waiting for the mesh
	 */
	void HandleActorAndMeshStopAndStartMoving();

	// ------------------------------------------------------------------
	// Other socket locations
	// ------------------------------------------------------------------

	/** Checks on what hight level player is at */
	/** The hight level is determined by the monster hight location and its sockets */
	/** Check mesh to see where the socket are */

	/** Checks if the player is below that socket's location to fire the appropiate weapon*/
	/** First hight level(the lowest one) to decide what weapon should be used for an attack */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sockets")
	FName FirstHightLevelSocketName;

	/** Checks if the player is below that socket's location to fire the appropiate weapon*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sockets")
	FName SecondHightLevelSocketName;

	/** Checks if the player is BELOW that socket's location to fire the appropiate weapon*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sockets")
	FName ThirdHightLevelSocketName;

	/** Checks if the player is ABOVE that socket's location to fire the appropiate weapon*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sockets")
	FName FourthHightLevelSocketName;

	/** Returns the world location of the hight level from the socket */
	/** Start with 1, ends with 4 */
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

	UFUNCTION(BlueprintImplementableEvent, Category = "Legs")
	void LegHasMoved(int32 Index,const FVector& Position);

	void InitLeg(class UMonsterLegComponent* Leg, int32 LegIndex);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
	bool bIsCurrentLegsOdd;



	// ------------------------------------------------------------------
	// Body Movement
	// ------------------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	FName BodySocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	FVector BodyPosition;

	UPROPERTY(EditDefaultsOnly, Category = "Body")
	float BodyUpMovementSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Body")
	bool bIsBodyMovingUp;

	UPROPERTY(EditDefaultsOnly, Category="Body")
	UCurveFloat* BodyFloatCurve;

	struct FTimeline BodyTimeline;

public:
	UFUNCTION(BlueprintCallable, Category = "Body")
	FVector GetCurrentBodyPosition();


	// ------------------------------------------------------------------
	// BODY MOVEMENT WHEN THERE IS AN OVERLAP FROM THE BODY TO THE LEGS
	// ------------------------------------------------------------------

	void ToggleWhatLegsShouldMove(const bool Odd);

	UPROPERTY( EditDefaultsOnly,BlueprintReadWrite, Category ="Body")
	TArray<FName> TopSocketLocationNames;


	UPROPERTY( EditDefaultsOnly,BlueprintReadWrite, Category ="Body")
	bool bBodyMoving;

	UPROPERTY( EditDefaultsOnly,BlueprintReadWrite, Category ="Body")
	float BodyForwardRaycastLength;

	UPROPERTY( EditDefaultsOnly,BlueprintReadWrite, Category ="Body")
	bool bCheckBodyAltitude;

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

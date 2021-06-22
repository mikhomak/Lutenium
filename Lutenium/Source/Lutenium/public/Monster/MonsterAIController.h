// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "MonsterAIController.generated.h"

/**
 *
 */
UCLASS()
class LUTENIUM_API AMonsterAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMonsterAIController();

    FORCEINLINE class UBlackboardComponent *GetBlackboardComp() { return BlackboardComp; }

    FORCEINLINE class UBehaviorTreeComponent *GetBehaviorTreeComp() { return BehaviorComp; }

    /* Caution! Can return nullptr! */
    UFUNCTION(BlueprintCallable, Category = "Weapons")
    class AMonsterWeapon *GetWeapon(EMonsterWeaponType MonsterWeaponType);

    /* Returns -1 in case if weapon was detatch*/
    UFUNCTION(BlueprintCallable, Category = "Weapons")
    int32 GetWeaponLevel(EMonsterWeaponType MonsterWeaponType);

    // ------------------------------------------------------------------
    // Movement
    // ------------------------------------------------------------------

    /**
     * Indicates if the monster can move
     * Checks for this value in the tick of AEnemyMonsterPawn
     * The monster moves when the current target hasn't been reached yet
     * Updates in StartMovingToNextTargetLocation() and StartOrStopMoving()
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
    bool bCanMove;

    /**
     * Position of the next target in World Space
     * When this position updates, we need to invoke AEnemyMonsterPawn::SetDirectionToMove(NextTargetPositionWS) to calculate new direction to move
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
    FVector NextTargetPositionWS;

    /**
     * Targets to reach
     * The order matters
     * Can be any type of Actor
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
    TArray<class AActor *> Targets;

    /**
     * Current Target index of array of Targets
     */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Movement")
    int32 CurrentTargetIndex;

    /**
     * Sets Targets and NextTargetPositionWS
     * @warning do not start the movement. Movement should be start with StartMoveToNextTargetLocation()
     * @warning Picks the first value of Targets for NextTargetPositionWS if 0 index is valid
     */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetTargetsAndNextPosition(UPARAM(ref) TArray<class AActor *> &NewTargets);

    /**
     * Sets next location to move to
     * Index valid or bCanMove will be set as false
     * bShouldMove sets bCanMove, only if the index is valid
     * If Index is valid and bShouldMove == true, invokes AEnemyMonsterPawn::SetDirectionToMove(NextTargetPositionWS)
     * Updates FN_BV_CurrentTargetIndex value in blackboard with Index
     * Updates FN_BV_bIsMonsterMoving with bCanMove
     * Invokes in Task
     * @param Index - next index of Targets
     * @param bShouldMove - should move or not. Updates bCanMove
     */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartMovingToNextTargetLocation(int32 Index, bool bShouldMove);

    /**
     * Updates the value of FN_BV_HasReachedCurrentTarget in blackboard
     * Invokes in Target Class (for now BP only)
     * Doesn't stop the movement, it should be done in behaviour tree
     * @param bHasReached - indicates if the monster has reached the target
     */
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetHasReachedCurrentTarget(bool bHasReached);

    /**
     * Stops or start the movement
     * Updates FN_BV_bIsMonsterMoving in Blackboard
     * Invokes in UBTTask_StopStartMoving
     */
    UFUNCTION(Category = "Movement")
    void StartOrStopMoving(bool bStartOrStopMoving);

    // ------------------------------------------------------------------
    // Player behaviour
    // ------------------------------------------------------------------

    /** Reference to the player */
    /** CAUTION! Can be nullptr if the player wasn't been found */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Behaviour")
    class APlayerPawn *Player;

    /** Checks if the player is int the radius to activate beam defense */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Behaviour")
    bool bIsPlayerInRadiusOfBeamDefense;

    /** Updates the value of the blackbord of IsPlayerInRadiusOfBeamDefense when it has changed */
    UFUNCTION(BlueprintCallable, Category = "Player Behaviour")
    void SetIsPlayerInRadiusOfBeamDefense(bool bIsInRadius);

    /** Gets plaer's hight level to determine which weapon should be executed */
    /** Use it in behaviour tasks */
    /** Depends on the world location of the socket levels of the mesh and the player's location */
    UFUNCTION(BlueprintCallable, Category = "Player Behaviour")
    int32 SetPlayerHightLevelBlackboardValue();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    virtual void OnPossess(APawn *InPawn) override;

    class UBehaviorTreeComponent *BehaviorComp;

    class UBlackboardComponent *BlackboardComp;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    class AEnemyMonsterPawn *MonsterPawn;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    class USkeletalMeshComponent *MonsterMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behaviour")
    FName FirstLevelSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behaviour")
    FName SecondLevelSocket;

    // ------------------------------------------------------------------
    // Blackboard value names
    // ------------------------------------------------------------------

    /** FORMAT OF VARIABLES  ----- FN_BV_VariableName */
    /** FName_BlackboardValue_VariableName */
    /** i like it, sue me, see if I care*/

    /** Checks if the player is in the radius of beam defense */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard value names")
    FName FN_BV_bIsPlayerInRadiusOfBeamDefense;

    /** Checks the hight level of the player */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard value names")
    FName FN_BV_PlayerHightLevel;

    /**
     * Index of the target of Targets array
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard value names")
    FName FN_BV_CurrentTargetIndex;

    /**
     * Bool indicatges if the monster has reached current target or not
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard value names")
    FName FN_BV_bHasReachedCurrentTarget;

    /**
     * Bool indicatges if the monster is moving or not
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blackboard value names")
    FName FN_BV_bIsMonsterMoving;
};

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


    FORCEINLINE class UBlackboardComponent* GetBlackboardComp() { return BlackboardComp; }

    FORCEINLINE class UBehaviorTreeComponent* GetBehaviorTreeComp() { return BehaviorComp; }

    /* Caution! Can return nullptr! */
    UFUNCTION(BlueprintCallable, Category = "Weapons")
    class AMonsterWeapon* GetWeapon(EMonsterWeaponType MonsterWeaponType);

    /* Returns -1 in case if weapon was detatch*/
    UFUNCTION(BlueprintCallable, Category = "Weapons")
    int32 GetWeaponLevel(EMonsterWeaponType MonsterWeaponType);

    // ------------------------------------------------------------------
    // Player behaviour
    // ------------------------------------------------------------------

    /** Reference to the player */
    /** CAUTION! Can be nullptr if the player wasn't been found */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Behaviour")
    class APlayerPawn* Player;

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

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Perception")
    class UAIPerceptionComponent* PerceptionComp;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Perception")
    class UAISenseConfig_Sight* SightConfig;

    virtual void OnPossess(APawn* InPawn) override;

    class UBehaviorTreeComponent* BehaviorComp;

    class UBlackboardComponent* BlackboardComp;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    class AEnemyMonsterPawn* MonsterPawn;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    class USkeletalMeshComponent* MonsterMesh;

    UPROPERTY( EditDefaultsOnly,BlueprintReadOnly, Category ="Behaviour")
    FName FirstLevelSocket;

    UPROPERTY( EditDefaultsOnly,BlueprintReadOnly, Category ="Behaviour")
    FName SecondLevelSocket;

    UFUNCTION(BlueprintCallable)
    void OnSeePlayer(TArray<class AActor*> Actors);

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
};

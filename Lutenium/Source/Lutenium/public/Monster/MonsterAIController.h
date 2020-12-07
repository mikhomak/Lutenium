// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Weapons/MonsterWeaponType.h"
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
    UFUNCTION(BlueprintCallable)
    class AMonsterWeapon* GetWeapon(EMonsterWeaponType MonsterWeaponType, Category = "Weapons");

    /* Returns -1 in case if weapon was detatch*/
    UFUNCTION(BlueprintCallable)
    class int32 GetWeaponLevel(EMonsterWeaponType MonsterWeaponType, Category = "Weapons");

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Perception")
    class UAIPerceptionComponent* PerceptionComp;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),  Category = "Perception")
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
};

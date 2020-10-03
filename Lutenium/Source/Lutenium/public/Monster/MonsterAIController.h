// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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

    virtual void OnPossess(APawn* InPawn) override;

    class UBehaviorTreeComponent* BehaviorComp;

    class UBlackboardComponent* BlackboardComp;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    class UPawnSensingComponent* PawnSensingComp;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    class AEnemyMonsterPawn* MonsterPawn;

    FORCEINLINE UBlackboardComponent* GetBlackboardComp() { return BlackboardComp; }

    FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComp() { return BehaviorComp; }


protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnSeePlayer(APawn* SeenPawn);

    UFUNCTION()
    void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);
};

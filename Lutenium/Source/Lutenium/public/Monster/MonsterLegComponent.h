// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyMonsterPawn.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"

#include "MonsterLegComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UMonsterLegComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMonsterLegComponent();

    class AEnemyMonsterPawn* EnemyMonsterPawn;

    UPROPERTY(EditAnywhere)
    class UCurveFloat* Curve;


protected:
    virtual void BeginPlay() override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;


    UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "2"))
    void SetRaycastLocation(const FVector& Location);

    UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "2"))
    FVector GetCurrentPosition() const;

    UFUNCTION(BlueprintCallable, meta = (AdvancedDisplay = "2"))
    void SetEnemyMonsterPawn(AEnemyMonsterPawn* MonsterPawn);

    UFUNCTION()
    void TimelineCallback();

    UFUNCTION()
    void TimelineFinished();

    void SetMonsterLegType(EMonsterLeg LegType);

    bool IsGrounded() const;

    void SetCanMove(const bool CanMove);

private:

    EMonsterLeg MonsterLegType;

    FVector RaycastLocation;

    void RaycastLeg();

    void CalculateZValue(float& ZValue);

    struct FTimeline LegTimeline;

    float MinTimeCurve;

    float MaxTimeCurve;

    FVector CurrentPosition;

    bool bMoving;

    bool bCanMove;

    bool bGrounded;

    float TimelineValue;

    float CurrentFloatTimelineValue;

    float HighestPoint;

    bool bHasReachedHighestPoint;

    float HighPointBetweenSteps;

    float LowestPointBetweenSteps;

    float LerpValue;

    float DistanceBetweenLegsToMove;

    float RaycastDownLength;

    FVector StartPosition;

    FVector FinishPosition;

    float GetCurrentValueForAxis(bool IsX);
};

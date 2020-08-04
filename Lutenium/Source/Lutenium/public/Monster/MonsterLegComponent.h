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

    void SetCanMove(const bool CanMove);

private:

    EMonsterLeg MonsterLegType;

    FVector RaycastLocation; // Location for raycast facing down (should be higher than a leg)

    void RaycastLeg();

    void CalculateZValue(float& ZValue);

    /*
     * Timeline Step
     * Time of the timeline - speed of the leg
     * Max Y value of the timeline - the highest point of the leg
     */
    struct FTimeline LegTimeline;

    float MinTimeCurve;

    float MaxTimeCurve;

    float TimelineValue; // Y value of the leg - Current value of the timeline(0-1 unclamped)

    float CurrentFloatTimelineValue; // X value of the leg - Current execution time 

    FVector CurrentPosition; // Current position of the leg

    bool bMoving; // Is leg moving

    bool bCanMove; // Can the leg move when its grounded

    float AddedYValue; // Y value added to the highest point in the step

    bool bHasReachedHighestPoint; // if the leg has reached the highest point

    float HighPointBetweenSteps; // Highest point in the step

    float LowestPointBetweenSteps; // Lowest point in the step

    float LerpValue; // LerpValue to lerp the position

    float DistanceBetweenLegsToMove;// Needed distance between the leg and the raycast to activate the timeline for moving

    float RaycastDownLength; // Down length of the vector of the raycast

    FVector StartPosition; // Start position of the step

    FVector FinishPosition; // Finish position of the step

    float GetCurrentValueForAxis(bool IsX);
};

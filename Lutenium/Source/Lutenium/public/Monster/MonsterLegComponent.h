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

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    class AEnemyMonsterPawn* EnemyMonsterPawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    class USkeletalMeshComponent* MonsterMesh;

    UPROPERTY(EditAnywhere)
    class UCurveFloat* Curve;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;
    void StartMovingLeg(FVector HitLocation);


    UFUNCTION()
    void TimelineCallback();

    UFUNCTION()
    void TimelineFinished();

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName FirstJointSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName SecondJointSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FName RaycastSocket;

    FORCEINLINE void SetMonsterMesh(class USkeletalMeshComponent* Mesh) { MonsterMesh = Mesh; }
    FORCEINLINE void SetEnemyMonsterPawn(class AEnemyMonsterPawn* Pawn) { EnemyMonsterPawn = Pawn; }
    FORCEINLINE FVector GetCurrentPosition() const { return CurrentPosition; }
    FORCEINLINE void SetMonsterLegType(EMonsterLeg LegType) { MonsterLegType = LegType; }
    FORCEINLINE void SetCanMove(const bool CanMove) { bCanMove = CanMove; }

protected:
    virtual void BeginPlay() override;


private:

    EMonsterLeg MonsterLegType;

    void RaycastLeg();

    void CalculateZValue(float& ZValue);

    FVector RaycastJoint(FVector& StartPos, FVector& EndPos,
                         FHitResult& HitResult, FCollisionQueryParams& CollisionParams);
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

    bool bStoppedOnObstacleBetweenJointAndDownRaycast; // If there was an obstacle between the second joint and the downraycast position and the leg has reached that position and stopped there

    float AddedYValue; // Y value added to the highest point in the step

    bool bHasReachedHighestPoint; // if the leg has reached the highest point

    float HighPointBetweenSteps; // Highest point in the step

    float LowestPointBetweenSteps; // Lowest point in the step

    float LerpValue; // LerpValue to lerp the position

    float DistanceBetweenLegsToMove;
    // Needed distance between the leg and the raycast to activate the timeline for moving

    float RaycastDownLength; // Down length of the vector of the raycast

    FVector StartPosition; // Start position of the step

    FVector FinishPosition; // Finish position of the step

    float GetCurrentValueForAxis(bool IsX);
};

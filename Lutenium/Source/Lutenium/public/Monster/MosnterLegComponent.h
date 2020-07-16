// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyMonsterPawn.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"

#include "MosnterLegComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UMosnterLegComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMosnterLegComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditAnywhere)
	class UCurveFloat *Curve;

	class AEnemyMonsterPawn* EnemyMonsterPawn;
	
	UFUNCTION(BlueprintCallable)
	void SetRaycastLocation(const FVector& Location);

	UFUNCTION(BlueprintCallable)
	FVector GeCurrentPosition() const;

	UFUNCTION(BlueprintCallable)
	void SetEnemyMonsterPawn(AEnemyMonsterPawn* MonsterPawn);
	
	
private:

	FVector RaycastLocation;

	void RaycastLeg();
	
	void TimelineCallback();

	void TimelineFinish();

	struct FTimeline LegTimeline;

	float MinTimeCurve;

	float MaxTimeCurve;

	FVector CurrentPosition;

	bool bCanMove;

	float TimelineValue;

	float CurrentFloatTimelineValue;

	float HighestPoint;

	float HighPointBetweenSteps;

	float LowestPointBetweenSteps;

	float LerpValue;

	float DistanceBetweenLegsToMove;

	float RaycastDownLength;

	FVector StartPosition;

	FVector FinishPosition;

	float GetCurrentValueForAxis(bool IsX);
};



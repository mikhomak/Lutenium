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

	class AEnemyMonsterPawn EnemyMonsterPawn;

	FVector RaycastLocation;

	struct FTimeline LegTimeline;

	FVector CurrentPosition;

	bool bCanMove;

private:

	void RaycastLeg();
	
	void TimelineCallback();

	void TimelineFinish();
};

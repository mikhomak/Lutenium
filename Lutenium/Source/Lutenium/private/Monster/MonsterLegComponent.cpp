#include "Monster/MonsterLegComponent.h"
#include "Monster/EnemyMonsterPawn.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"

UMonsterLegComponent::UMonsterLegComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bMoving = false;
}

void UMonsterLegComponent::BeginPlay()
{
	Super::BeginPlay();
	/** Gets default values from the EnemyMonsterPawn */
	if(EnemyMonsterPawn)
	{
		DistanceBetweenLegsToMove = EnemyMonsterPawn->LegDistanceBetweenLegsToMove;
		RaycastDownLength = EnemyMonsterPawn->LegRaycastDownLength;
		LerpValue = EnemyMonsterPawn->LegLerpValue;
		AddedHightStep = EnemyMonsterPawn->LegAddedHightStep;
		StepTime = EnemyMonsterPawn->LegStepTime;
	}

	CurrentStepTime = 0.f;

	/* Sets default location for each leg no matter if it can move or not */
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(EnemyMonsterPawn);
	FVector RaycastLocation = MonsterMesh->GetSocketLocation(RaycastSocket); /* Gets location of the raycast socket */
	FVector RaycastEndLocation = RaycastLocation + (FVector::DownVector * RaycastDownLength); /* Downvector from this socket  */
	FVector DownRaycast = Raycast(RaycastLocation,
										RaycastEndLocation,
										HitResult, CollisionParams);
	if(HitResult.bBlockingHit)
	{
		CurrentPosition = DownRaycast;
	}
}


void UMonsterLegComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RaycastLeg();
	if(bMoving)
	{
		MoveLeg(DeltaTime);
	}
}


void UMonsterLegComponent::RaycastLeg()
{
	if (bMoving || !bCanMove)
	{
		return;
	}

	/* Raycasting only if the socket has moved too far away from the current leg position */
	FVector RaycastLocation = MonsterMesh->GetSocketLocation(RaycastSocket);
	FVector RaycastEndLocation = RaycastLocation + (FVector::DownVector * RaycastDownLength);
	const float DistanceBetweenCurrentPosAndPrevious = FVector::Distance(
		CurrentPosition, FVector(RaycastLocation.X, RaycastLocation.Y, CurrentPosition.Z));

	DEBUG_DrawLineBetweenPoints(CurrentPosition, RaycastLocation);
	DEBUG_DrawRaycastSphere();
	if (DistanceBetweenCurrentPosAndPrevious >= DistanceBetweenLegsToMove)
	{
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(EnemyMonsterPawn);
		FVector RaycastResultLocation;
		RaycastPosition = Raycast(RaycastLocation,
										   RaycastEndLocation,
										   HitResult, CollisionParams);

		/* If the position wasn't found, do nothing */
		if (HitResult.bBlockingHit)
		{
			/* If no obstacles were found, then move the leg to the finish position */
			StartMovingLeg(RaycastPosition);
		}
	}
}

FVector UMonsterLegComponent::Raycast(FVector& StartPos, FVector& EndPos, FHitResult& HitResult,
										   FCollisionQueryParams& CollisionParams)
{
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPos,
		EndPos,
		ECollisionChannel::ECC_WorldStatic,
		CollisionParams);
	return HitResult.ImpactPoint;
}



void UMonsterLegComponent::StartMovingLeg(const FVector HitLocation)
{
	StartPosition = CurrentPosition;
	FinishPosition = HitLocation;

	HighestPoint = FMath::Max(StartPosition.Z, FinishPosition.Z) + AddedHightStep;

	bMoving = true;

	FTimerHandle StepHandler;
	GetWorld()->GetTimerManager().SetTimer(StepHandler, this, &UMonsterLegComponent::StopMoving, StepTime, false);
}

void UMonsterLegComponent::StopMoving()
{
	bMoving = false;
	CurrentStepTime = 0.f;
	bHasReachedHighestPoint = false;
	EnemyMonsterPawn->LegHasMovedEventCaller(LegIndex);
}

void UMonsterLegComponent::MoveLeg(float DeltaTime)
{
	CurrentStepTime += DeltaTime;
	CurrentPosition = FMath::Lerp(CurrentPosition, GetCurrentPositionOfStep(), LerpValue);
}


FVector UMonsterLegComponent::GetCurrentPositionOfStep()
{


	const float X = FMath::GetMappedRangeValueClamped(
							FVector2D(0, StepTime),
							FVector2D(StartPosition.X, FinishPosition.X),
							CurrentStepTime);
	const float Y = FMath::GetMappedRangeValueClamped(
							FVector2D(0, StepTime),
							FVector2D(StartPosition.Y, FinishPosition.Y),
							CurrentStepTime);

	// Z value defines depending if CurrentPosition.Z is higher of FinishPosition.Z
	// The parabola of the step takes always StepTime seconds, so to get to the middle of the parabola, even if the leg should start on the higher ground, it takes StepTime/2
	// Same goes when the FinishPosition is lower than StartPosition
	//  CASE 1:
	//
	//						   HighestPoint
	//				             ______
	//          			   /       \
	// 				          /         \
	//         				 /           \
	//				        /             \
	//				       /               \
	//     				  /                 \
	// 	  FinishPosition /                   \ CurrentPosition
	// -------------------------------------------------
	//
	//
	// CASE 2:
	//
	//						   HighestPoint
	//				             ______
	//          			   /       \
	// 				          /         \
	//        FinishPosition /           \
	//				      _____           \
	//				     |     |           \
	//     				 |     |            \
	//                   |     |             \ CurrentPosition
	// -------------------------------------------------
	//
	//
	//  CASE 3:
	//
	//						   HighestPoint
	//				             ______
	//          			   /       \
	// 				          /         \
	//         				 /           \ CurrentPosition
	//				        /            ____
	//				       /           |     |
	//     				  /            |     |
	// 	  FinishPosition /             |     |
	// -------------------------------------------------

	if (CurrentPosition.Z > HighestPoint - 50.f)
	{
		bHasReachedHighestPoint = true;
	}

	const float Z = bHasReachedHighestPoint == false ?
				 FMath::GetMappedRangeValueClamped(
							FVector2D(0, StepTime / 2),
							FVector2D(StartPosition.Z, HighestPoint),
							CurrentStepTime) :
				FMath::GetMappedRangeValueClamped(
							FVector2D(StepTime / 2, StepTime),
							FVector2D(HighestPoint, FinishPosition.Z),
							CurrentStepTime);
	return FVector(X, Y, Z);
}




void UMonsterLegComponent::DEBUG_DrawRaycastSphere()
{
	if(!bDrawDebug)
	{
		return;
	}
	DrawDebugSphere(GetWorld(),       // world
					RaycastPosition,  // position
					500.f,			  // radius
					6,                // segments
					FColor::Red, 	  // Color
					false,			  // persistent lines
					0.f,			  // life time
					0,             	  // depth
					150.f);			  // thicknes
}

void UMonsterLegComponent::DEBUG_DrawLineBetweenPoints(FVector& StartLocation, FVector& EndLocation)
{
	if(!bDrawDebug)
	{
		return;
	}

	DrawDebugLine
	(
		GetWorld(),			// world
		StartLocation,		// start location
		EndLocation,		// end location
		FColor::Red,		// color
		false,				// persistent lines
		0.f,				// life time
		0,					// depth
		150.f 				// thicknes
	);
}
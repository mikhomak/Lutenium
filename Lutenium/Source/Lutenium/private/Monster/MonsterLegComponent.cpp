#include "Monster/MonsterLegComponent.h"
#include "Monster/EnemyMonsterPawn.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"

UMonsterLegComponent::UMonsterLegComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bMoving = false;

	//General
	RaycastSphereRadius = 4000.f;

	// obstacle movement
	bShouldRaycastJointsWhileMoving = true;
	LerpAlphaValueMovingLegWhenThereIsAnObstacle = 0.4f;

	bActivateFirstJointRaycast = true;
	bActivateSecondJointRaycast = true;

	MaxAttempsAtSecondJointRaycast = 2;
	CurrentAttempsAtSecondJointRaycast = 0;
	MaxAttempsAtThirdJointRaycast = 2;
	CurrentAttempsAtThirdJointRaycast = 0;

	// DEBUG
	DEBUG_Thickness = 1500.f;
	DEBUG_LifeTime = 5.f;
	DEBUG_SphereRadius = 5000.f;
}

void UMonsterLegComponent::BeginPlay()
{
	Super::BeginPlay();
	/** Gets default values from the EnemyMonsterPawn */
	if (EnemyMonsterPawn)
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
	FVector RaycastLocation = GetComponentLocation();										  /* Gets location of the raycast socket */
	FVector RaycastEndLocation = RaycastLocation + (FVector::DownVector * RaycastDownLength); /* Downvector from this socket  */
	FVector DownRaycast = Raycast(RaycastLocation,
								  RaycastEndLocation,
								  HitResult);
	if (HitResult.bBlockingHit)
	{
		CurrentPosition = DownRaycast;
	}
}

void UMonsterLegComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										 FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	RaycastLeg();
	if (bMoving)
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
	const FVector RaycastLocation = GetComponentLocation();
	const FVector RaycastEndLocation = RaycastLocation + (FVector::DownVector * RaycastDownLength);
	const float DistanceBetweenCurrentPosAndPrevious = FVector::Distance(
		CurrentPosition, FVector(RaycastLocation.X, RaycastLocation.Y, CurrentPosition.Z));

	// DEBUG
	if (bDrawDebug)
	{
		DEBUG_DrawLineBetweenPoints(
			FVector(CurrentPosition.X, CurrentPosition.Y, RaycastLocation.Z),
			RaycastLocation,
			FColor::Yellow);
		DEBUG_DrawRaycastSphere();
	}

	// Main Logic
	if (DistanceBetweenCurrentPosAndPrevious >= DistanceBetweenLegsToMove)
	{
		// Joint raycasting
		// IMPORTANT, inside we check for the bActivateFirstJointRaycast and bActivateSecondJointRaycast
		// No need to check it here altough you can idc
		if (RaycastFirstJoint())
		{
			return;
		}

		if (RaycastSecondJoint())
		{
			return;
		}

		// Normal raycast
		RaycastWithStartMoving(RaycastLocation, RaycastEndLocation);
	}
}

bool UMonsterLegComponent::RaycastFirstJoint()
{
	if (!bActivateFirstJointRaycast)
	{
		return false;
	}
	if (Mesh == nullptr)
	{
		return false;
	}

	if (CurrentAttempsAtSecondJointRaycast > MaxAttempsAtSecondJointRaycast)
	{
		CurrentAttempsAtSecondJointRaycast = 0;
		return false;
	}

	CurrentAttempsAtSecondJointRaycast += 1;
	/**
     *  Sockets to raycast
     *                 THOSE ARE THE LEGS OKAY?
     *
     *
     *                  FRONT  MIDDLE  BACK
     *
     *                    /2\    |    /2\
     *                   /   \   |   /   \
     *                  /     \  |  /     \
     *                 /     MONSTER       \
     *                /          |          \
     *               /           |           \
     *              3            |            3
     *---------------------------------------------- FLOOR
    */
	const FVector RaycastLocation = EnemyMonsterPawn->GetCurrentBodyPosition();
	const FVector RaycastEndLocation = Mesh->GetSocketLocation(SecondJointSocketName);
	return RaycastWithStartMoving(RaycastLocation, RaycastEndLocation);
}

bool UMonsterLegComponent::RaycastSecondJoint()
{
	if (!bActivateSecondJointRaycast)
	{
		return false;
	}
	if (Mesh == nullptr)
	{
		return false;
	}

	if (CurrentAttempsAtThirdJointRaycast > MaxAttempsAtThirdJointRaycast)
	{
		CurrentAttempsAtThirdJointRaycast = 0;
		return false;
	}

	CurrentAttempsAtThirdJointRaycast += 1;
	/**
     *  Sockets to raycast
     *                 THOSE ARE THE LEGS OKAY?
     *
     *
     *                  FRONT  MIDDLE  BACK
     *
     *                    /2\    |    /2\
     *                   /   \   |   /   \
     *                  /     \  |  /     \
     *                 /     MONSTER       \
     *                /          |          \
     *               /           |           \
     *              3            |            3
     *---------------------------------------------- FLOOR
    */
	const FVector RaycastLocation = Mesh->GetSocketLocation(SecondJointSocketName);
	const FVector RaycastEndLocation = Mesh->GetSocketLocation(ThirdJointSocketName);
	return RaycastWithStartMoving(RaycastLocation, RaycastEndLocation);
}

bool UMonsterLegComponent::RaycastWithStartMoving(const FVector &StartPos, const FVector &EndPos)
{
	DEBUG_DrawLineBetweenPoints(StartPos, EndPos, FColor::Cyan);
	FHitResult HitResult;
	RaycastPosition = Raycast(StartPos,
							  EndPos,
							  HitResult);

	/* If the position wasn't found, do nothing */
	if (HitResult.bBlockingHit)
	{
		/* If no obstacles were found, then move the leg to the finish position */
		StartMovingLeg(RaycastPosition);
		return true;
	}
	return false;
}

FVector UMonsterLegComponent::Raycast(const FVector &StartPos, const FVector &EndPos, FHitResult &HitResult)
{
	DEBUG_DrawSphere(StartPos, FColor::Blue);
	DEBUG_DrawSphere(EndPos, FColor::Green);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(EnemyMonsterPawn);
	GetWorld()->SweepSingleByObjectType(
		HitResult,
		StartPos,
		EndPos,
		FQuat::Identity,
		ECollisionChannel::ECC_WorldStatic,
		FCollisionShape::MakeSphere(RaycastSphereRadius),
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
	ObstacleHitPosition = FVector::ZeroVector;
	bIsThereAnObstacle = false;
	EnemyMonsterPawn->LegHasMovedEventCaller(LegIndex, CurrentPosition);
}

void UMonsterLegComponent::MoveLeg(float DeltaTime)
{
	CurrentStepTime += DeltaTime;

	// cautiion!
	// this can change FinishPosition!!!
	RaycastSecondJointsWhileMoving();

	CurrentPosition = FMath::Lerp(CurrentPosition, GetCurrentPositionOfStep(), LerpValue);
}

/**
 * While moving doing some raycast shit to see if there is an obstacle inside of the leg
 */
void UMonsterLegComponent::RaycastSecondJointsWhileMoving()
{
	if (!bShouldRaycastJointsWhileMoving)
	{
		return;
	}

	// if the obstacle has been already found, lerps the FinishPosition to Obstacle Position
	if (bIsThereAnObstacle)
	{
		FinishPosition = FMath::Lerp(FinishPosition, ObstacleHitPosition, LerpAlphaValueMovingLegWhenThereIsAnObstacle);
	}
	else
	{
		FHitResult HitResult;
		const FVector SecondJointLocation = Mesh->GetSocketLocation(SecondJointSocketName);
		const FVector ThridJointSocketLocation = Mesh->GetSocketLocation(ThirdJointSocketName);
		FVector ObstaclePosition = Raycast(SecondJointLocation, ThridJointSocketLocation, HitResult);
		if (HitResult.bBlockingHit)
		{
			ObstacleHitPosition = ObstaclePosition;
			bIsThereAnObstacle = true;
		}
	}
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

	const float Z = bHasReachedHighestPoint == false ? FMath::GetMappedRangeValueClamped(
														   FVector2D(0, StepTime / 2),
														   FVector2D(StartPosition.Z, HighestPoint),
														   CurrentStepTime)
													 : FMath::GetMappedRangeValueClamped(
														   FVector2D(StepTime / 2, StepTime),
														   FVector2D(HighestPoint, FinishPosition.Z),
														   CurrentStepTime);
	return FVector(X, Y, Z);
}

void UMonsterLegComponent::DEBUG_DrawRaycastSphere()
{
	if (!bDrawDebug)
	{
		return;
	}
	DrawDebugSphere(GetWorld(),			// world
					RaycastPosition,	// position
					DEBUG_SphereRadius, // radius
					6,					// segments
					FColor::Red,		// Color
					false,				// persistent lines
					DEBUG_LifeTime,		// life time
					0,					// depth
					DEBUG_Thickness);	// thicknes
}

void UMonsterLegComponent::DEBUG_DrawLineBetweenPoints(const FVector &StartLocation, const FVector &EndLocation, FColor Color)
{
	if (!bDrawDebug)
	{
		return;
	}

	DrawDebugLine(
		GetWorld(),		// world
		StartLocation,	// start location
		EndLocation,	// end location
		Color,			// color
		false,			// persistent lines
		DEBUG_LifeTime, // life time
		0,				// depth
		DEBUG_Thickness // thicknes
	);
}

void UMonsterLegComponent::DEBUG_DrawSphere(const FVector &Location, FColor Color)
{
	if (!bDrawDebug)
	{
		return;
	}
	DrawDebugSphere(GetWorld(),			// world
					RaycastPosition,	// position
					DEBUG_SphereRadius, // radius
					6,					// segments
					Color,				// Color
					false,				// persistent lines
					DEBUG_LifeTime,		// life time
					0,					// depth
					DEBUG_Thickness);	// thicknes
}

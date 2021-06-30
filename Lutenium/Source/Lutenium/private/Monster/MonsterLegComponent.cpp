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
	bShouldCheckForTheDistanceBetweenLegsToMove = true;

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

	if(bDrawDebug)
	{
		DEBUG_DrawLineBetweenPoints(
			CurrentPosition,
			GetComponentLocation(),
			FColor::Yellow,
			true);

		const FVector FirstJointRaycastLocation = EnemyMonsterPawn->GetCurrentBodyPosition();
		const FVector FirstJointRaycastEndLocation = Mesh->GetSocketLocation(SecondJointSocketName);
		const FVector RaycastEndLocation = GetComponentLocation() + (FVector::DownVector * RaycastDownLength);

		// 				JOINTS
		// first joint
		DEBUG_DrawLineBetweenPoints(
			FirstJointRaycastLocation,
			FirstJointRaycastEndLocation,
			FColor::Purple,
			true);

		const FVector SecondJointRaycastEndLocation = Mesh->GetSocketLocation(ThirdJointSocketName);

		// second joint
		DEBUG_DrawLineBetweenPoints(
			FirstJointRaycastEndLocation,
			SecondJointRaycastEndLocation,
			FColor::Purple,
			true);

		//	second joint to finish position
		DEBUG_DrawLineBetweenPoints(
			FirstJointRaycastEndLocation,
			FinishPosition,
			FColor::Orange,
			true);

		// second joint to raycast position
		DEBUG_DrawLineBetweenPoints(
			FirstJointRaycastEndLocation,
			RaycastEndLocation,
			FColor::Emerald,
			true);


		// 				FINISH POSITION
		DEBUG_DrawLineBetweenPoints(
			CurrentPosition,
			FinishPosition,
			FColor::Red,
			true);

		DEBUG_DrawSphere(
			FinishPosition,
			FColor::Red,
			true);

		// Raycast position
		DEBUG_DrawLineBetweenPoints(
			FirstJointRaycastEndLocation,
			RaycastPosition,
			FColor::Red,
			true);

		// Raycast To the highest step position
		const float HighestZLocation = FMath::Max(CurrentPosition.Z, RaycastPosition.Z) + AddedHightStep;
		FVector MiddlePosition = FMath::Lerp(CurrentPosition, RaycastPosition, 0.5f);
		MiddlePosition.Z = HighestZLocation;
		DEBUG_DrawLineBetweenPoints(
			StartPosition,
			MiddlePosition,
			FColor::Blue,
			true);

	}
}

void UMonsterLegComponent::RaycastLeg()
{
	if (bMoving || !bCanMove)
	{
		return;
	}

	/* Raycasting only if the socket has moved too far away from the current leg position */
	const FVector ComponentLocation = GetComponentLocation();
	const FVector RaycastEndLocation = ComponentLocation + (FVector::DownVector * RaycastDownLength);
	const float DistanceBetweenCurrentPosAndPrevious = FVector::Distance(
		CurrentPosition, FVector(ComponentLocation.X, ComponentLocation.Y, CurrentPosition.Z));

	// Main Logic
	if (!bShouldCheckForTheDistanceBetweenLegsToMove || DistanceBetweenCurrentPosAndPrevious >= DistanceBetweenLegsToMove)
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
		FHitResult HitResult;
		RaycastPosition = Raycast(ComponentLocation,
								  RaycastEndLocation,
								  HitResult);
		if(HitResult.bBlockingHit)
		{


		   /**
 		    *  Sockets to raycast
 		    * Case 1: No obstacle, move the leg to the raycast position (return false -> doesn't start moving)
 		    *       
 		    *       
 		    *
 		    *                   RAYCAST LOCATION
 		    *                  	     |	   
 		    *                        |              FRONT  MIDDLE  BACK
 		    *                        |   
 		    *                        |                /2\    |    /2\
 		    *      					 |               /   \   |   /   \
 		    *                        |              /     \  |  /     \
 		    *                        |             /     MONSTER       \
 		    *                        |            /          |          \
 		    *                        |           /           |           \
 		    *                        |          3            |            3
 		    *--------------------------------------------------------------- FLOOR
 		    *                     RAYCAST POSITION     
 		    *
 		    *
 		    *
 		    * Case 2: There is an obstacle, move to the obstacle location (return true -> start moving)
 		    *       
 		    *
 		    *                   RAYCAST LOCATION
 		    *                  	     |	   
 		    *                        |              FRONT  MIDDLE  BACK
 		    *                        |   
 		    *                        |                /2\    |    /2\
 		    *      					 | OBSTACLE      /   \   |   /   \
 		    *                        |  -----       /     \  |  /     \
 		    *                        |  |   |      /     MONSTER       \
 		    *                        |  |   |     /          |          \
 		    *                        |  |   |    /           |           \
 		    *                        |  |   |   3            |            3
 		    *--------------------------------------------------------------- FLOOR
 		    *                     RAYCAST POSITION         
 		    */
			// Should we raycast from the second joint to the raycast end position?
			if(RaycastSecondJointToRaycastPosition(ComponentLocation, RaycastPosition))
			{
				return;
			}
			// If there was no obstacle between second joint and end postion, start moving to the end position
			StartMovingLeg(RaycastPosition);
			RaycastToTheHighestPosition(StartPosition, FinishPosition);
		}
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


bool UMonsterLegComponent::RaycastSecondJointToRaycastPosition(const FVector& StartPos, const FVector& EndPos)
{
	// safety checks
	if(!bShouldCheckForAnObstacleFromSecdonJointToRaycastPosition)
	{
		return false;
	}
	if (Mesh == nullptr)
	{
		return false;
	}
    /**
     *  Sockets to raycast
     * Case 1: No obstacle, move the leg to the raycast position (return false -> doesn't start moving)
     *       
     *       
     *
     *                   RAYCAST LOCATION
     *                  	  |	   
     *                        |              FRONT  MIDDLE  BACK
     *                        |   
     *                        |                /2\    |    /2\
     *      				  |               /   \   |   /   \
     *                        |              /     \  |  /     \
     *                        |             /     MONSTER       \
     *                        |            /          |          \
     *                        |           /           |           \
     *                        |          3            |            3
     *--------------------------------------------------------------- FLOOR
     *                     RAYCAST POSITION     
     *
     *
     *
     * Case 2: There is an obstacle, move to the obstacle location (return true -> start moving)
     *       
     *
     *                   RAYCAST LOCATION
     *                  	  |	   
     *                        |              FRONT  MIDDLE  BACK
     *                        |   
     *                        |                /2\    |    /2\
     *      				  | OBSTACLE      /   \   |   /   \
     *                        |  -----       /     \  |  /     \
     *                        |  |   |      /     MONSTER       \
     *                        |  |   |     /          |          \
     *                        |  |   |    /           |           \
     *                        |  |   |   3            |            3
     *--------------------------------------------------------------- FLOOR
     *                     RAYCAST POSITION         
     */

	return RaycastWithStartMoving(StartPos, EndPos);

}

bool UMonsterLegComponent::RaycastToTheHighestPosition(const FVector &StartPos, const FVector& EndPos)
{
	// safety checks
	if(!bRaycastToTheHighestStep)
	{
		return false;
	}
	if(Mesh == nullptr)
	{
		return false;
	}

	// Setting Highest point
	const float HighestZLocation = FMath::Max(StartPos.Z, EndPos.Z) + AddedHightStep;
	FVector MiddlePosition = FMath::Lerp(StartPos, EndPos, 0.5f);
	MiddlePosition.Z = HighestZLocation;
	FHitResult HitResult;
	FVector RaycastResult = Raycast(StartPos, MiddlePosition, HitResult);
	if(HitResult.bBlockingHit)
	{
		FinishPosition = RaycastResult;
		return true;
	}
	return false;
}


bool UMonsterLegComponent::RaycastWithStartMoving(const FVector &StartPos, const FVector &EndPos)
{
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

void UMonsterLegComponent::DEBUG_DrawRaycastSphere(bool bDrawEveryThick)
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
					bDrawEveryThick ? 0.f : DEBUG_LifeTime,		// life time
					0,					// depth
					DEBUG_Thickness);	// thicknes
}

void UMonsterLegComponent::DEBUG_DrawLineBetweenPoints(const FVector &StartLocation, const FVector &EndLocation, FColor Color, bool bDrawEveryThick)
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
		bDrawEveryThick ? 0.f : DEBUG_LifeTime, // life time
		0,				// depth
		DEBUG_Thickness // thicknes
	);
}

void UMonsterLegComponent::DEBUG_DrawSphere(const FVector &Location, FColor Color, bool bDrawEveryThick)
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
					bDrawEveryThick ? 0.f : DEBUG_LifeTime,		// life time
					0,					// depth
					DEBUG_Thickness);	// thicknes
}

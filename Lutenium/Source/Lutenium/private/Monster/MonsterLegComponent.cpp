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
	LerpAlphaValueMovingLegWhenThereIsAnObstacle = 0.4f;
	BodyBoneName = "Body";

	// Directional obstacle
	AddedHightForRaycastForDirectionalObstacle = 35000.f;
	RaycastDirectionalVectorDistance = 20000.f;
	DirectionalRaycastBeggingThreshold =0.2f;
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
	FVector RaycastLocation = GetComponentLocation();										  
	FVector RaycastEndLocation = RaycastLocation + (FVector::DownVector * RaycastDownLength); 
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
		DEBUG_DrawSphere(
			GetComponentLocation(),
			FColor::Orange,
			true);

		DEBUG_DrawSphere(
			StartPosition,
			FColor::Blue,
			true);

		DEBUG_DrawSphere(
			FinishPosition,
			FColor::Red,
			true);



		FVector GoalPosition;
		// the leg is still going up
		// lookin for middle point of the step
		if(!bHasReachedHighestPoint)
		{
			const float HighestZLocation = FMath::Max(StartPosition.Z, FinishPosition.Z) + AddedHightStep;
			GoalPosition = (StartPosition + FinishPosition) / 2;
			GoalPosition.Z = HighestZLocation;
		}
		// the leg is going down
		// looking for the finish point of the step
		else
		{
			GoalPosition = FinishPosition;
		}
		GoalPosition = GoalPosition - CurrentPosition;
		GoalPosition.Normalize();
		GoalPosition = GoalPosition * RaycastDirectionalVectorDistance;




		DEBUG_DrawLineBetweenPoints(
			CurrentPosition,
			CurrentPosition + GoalPosition,
			FColor::Red,
			true);

		DEBUG_DrawLineBetweenPoints(
			CurrentPosition,
			FinishPosition,
			FColor::Purple,
			true);





	}
}

void UMonsterLegComponent::RaycastLeg()
{
	if (bMoving || !bCanMove)
	{
		return;
	}

	/* Raycasting only if the component has moved too far away from the current leg position */
	const FVector ComponentLocation = GetComponentLocation();
	const FVector RaycastEndLocation = ComponentLocation + (FVector::DownVector * RaycastDownLength);
	const float DistanceBetweenCurrentPosAndPrevious = FVector::Distance(
		CurrentPosition, FVector(ComponentLocation.X, ComponentLocation.Y, CurrentPosition.Z));

	// Main Logic
	if (!bShouldCheckForTheDistanceBetweenLegsToMove || DistanceBetweenCurrentPosAndPrevious >= DistanceBetweenLegsToMove)
	{
		// Joint raycasting
		// IMPORTANT, inside we check for the bShouldRaycastAllJoints
		// No need to check it here altough you can idc
		if(RaycastJointArray())
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
			// If there was no obstacle between second joint and end postion, start moving to the end position
			StartMovingLeg(RaycastPosition);
			RaycastToTheHighestPosition(StartPosition, FinishPosition);
		}
	}
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
	}else
	{
		RaycastResult = Raycast(MiddlePosition, EndPos, HitResult);
		if(HitResult.bBlockingHit)
		{
			FinishPosition = RaycastResult;
			return true;
		}
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


bool UMonsterLegComponent::RaycastJointArray()
{
	if(!bShouldRaycastAllJoints)
	{
		return false;
	}
	if(Mesh == nullptr)
	{
		return false;
	}

	bool result = false;
	for(int i = 0; i < JointRaycastArray.Num(); i++)
	{
		// if i + 1 is not valid, that mean that we reached the final bone and thus no hit was found
		if(!JointRaycastArray.IsValidIndex(i+1))
		{
			return false;
		}

		const FVector StartPos = Mesh->GetBoneLocation(JointRaycastArray[i], EBoneSpaces::WorldSpace);
		const FVector EndPos = Mesh->GetBoneLocation(JointRaycastArray[i+1], EBoneSpaces::WorldSpace);

		result = RaycastWithStartMoving(StartPos, EndPos);
		if(result == true)
		{
			return true;
		}
	}
	return false;
}


/**
 * Raycast method for every raycast in this class
 */
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

	// directional raycast
	bCanRaycastDirection = false;
	FTimerHandle DirectionalRaycastThresholdTimerHandler;
	GetWorld()->GetTimerManager().SetTimer(DirectionalRaycastThresholdTimerHandler, this, &UMonsterLegComponent::ResetCanRayscastDirection,DirectionalRaycastBeggingThreshold,false);
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
	RaycastAllJointsWhileMoving();
	RaycastWhileMovingForDirectionalObstacle();



	CurrentPosition = FMath::Lerp(CurrentPosition, GetCurrentPositionOfStep(), LerpValue);
}

bool UMonsterLegComponent::RaycastWhileMovingForDirectionalObstacle()
{
	if(!bShouldRaycastForDirectionalObstacle || !bCanRaycastDirection)
	{
		return false;
	}

	FVector GoalPosition;
	// the leg is still going up
	// lookin for middle point of the step
	if(!bHasReachedHighestPoint)
	{
		const float HighestZLocation = FMath::Max(StartPosition.Z, FinishPosition.Z) + AddedHightStep;
		GoalPosition = (StartPosition + FinishPosition) / 2;
		GoalPosition.Z = HighestZLocation;
	}
	// the leg is going down
	// looking for the finish point of the step
	else
	{
		GoalPosition = FinishPosition;
	}
	GoalPosition = GoalPosition - CurrentPosition;
	GoalPosition.Normalize();
	GoalPosition = GoalPosition * RaycastDirectionalVectorDistance;



	FHitResult HitResult;
	const FVector DirectionalObstaclePosition = RaycastObstaclePositionFromTheDownVector(CurrentPosition + GoalPosition, HitResult);
	if(HitResult.bBlockingHit)
	{
		FinishPosition = DirectionalObstaclePosition;
		return true;
	}

	return false;
}


FVector UMonsterLegComponent::RaycastObstaclePositionFromTheDownVector(const FVector& GoalPosition, FHitResult& HitResult)
{
	FVector DirectionalObstacleHitPosition = Raycast(CurrentPosition, GoalPosition, HitResult);
	if(HitResult.bBlockingHit)
	{
		FVector FinishDirectionalObstacleHitPosition = Raycast(DirectionalObstacleHitPosition + FVector::UpVector * AddedHightForRaycastForDirectionalObstacle, DirectionalObstacleHitPosition + FVector::DownVector * RaycastDownLength, HitResult);
		if(HitResult.bBlockingHit)
		{
			return FinishDirectionalObstacleHitPosition;
		}
	}

	return FVector::ZeroVector;
}



bool UMonsterLegComponent::RaycastAllJointsWhileMoving()
{
	if(!bShouldRaycastAllJointsWhileMoving)
	{
		return false;
	}

	// if there is already an obstacle, we don't care about raycasting all joints
	// we only want to lerp the FinishPosition with ObstacleHitPosition
	// that happens inside of RaycastWhileMoving()
	// and we don't care about input vectors
	if(bIsThereAnObstacle)
	{
		RaycastWhileMoving(FVector(), FVector());
		return true;
	}
	
	bool result = false;
	for(int i = 0; i < JointRaycastArray.Num(); i++)
	{
		// if i + 1 is not valid, that mean that we reached the final bone and thus no hit was found
		if(!JointRaycastArray.IsValidIndex(i+1))
		{
			return false;
		}

		const FVector StartPos = Mesh->GetBoneLocation(JointRaycastArray[i], EBoneSpaces::WorldSpace);
		const FVector EndPos = Mesh->GetBoneLocation(JointRaycastArray[i+1], EBoneSpaces::WorldSpace);

		result = RaycastWhileMoving(StartPos, EndPos);
		if(result == true)
		{
			return true;
		}
	}

	return false;

}


bool UMonsterLegComponent::RaycastWhileMoving(const FVector& StartPos, const FVector& EndPos)
{
	// if the obstacle has been already found, lerps the FinishPosition to Obstacle Position
	if (bIsThereAnObstacle)
	{
		FinishPosition = FMath::Lerp(FinishPosition, ObstacleHitPosition, LerpAlphaValueMovingLegWhenThereIsAnObstacle);
		return true;
	}
	else
	{
		FHitResult HitResult;
		FVector ObstaclePosition = Raycast(StartPos, EndPos, HitResult);
		if (HitResult.bBlockingHit)
		{
			if(IsPositionInTheExcludedArea(ObstaclePosition))
			{
				return false;
			}
			ObstacleHitPosition = ObstaclePosition;
			bIsThereAnObstacle = true;
			return true;
		}
	}
	return false;

}

bool UMonsterLegComponent::IsPositionInTheExcludedArea(FVector& Position)
{
	if(!bShouldExcludeBodyArea)
	{
		return false;
	}

	FVector BodyPosition = Mesh->GetBoneLocation(BodyBoneName, EBoneSpaces::WorldSpace);
	if(FVector::Distance(BodyPosition, Position) < ExcludedBodyAreaRadius)
	{
		return true;
	}
	return false;
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

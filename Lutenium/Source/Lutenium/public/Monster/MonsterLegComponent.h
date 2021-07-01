#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MonsterLegComponent.generated.h"

/**
 * This component calculates the position for the leg in each moment
 * Main output of this component is the method GetCurrentPosition(), which returns the position where the leg is supposted to be
 * This method is called in EnemyMonsterPawn and then is transefered to the animation blueprint, where the bone of the moves to that location
 *
 * How it works:
 *		1) Checks if the CurrentPosition and the component position is far enough to start a step(DistanceBetweenCurrentPosAndPrevious >= DistanceBetweenLegsToMove)
 *      2) If so, raycast down from the component location
 *		3) If there is a blocking hit, starts moving with the StartMovingLeg(RaycastHitBlockingPosition)
 *		4) This method starts a movement (bMoving = true) and sets timer to stop the step StopMoving()
 *		5) While bMoving == true -> each tick modifies CurrentPosition in MoveLeg()
 *
 * Steps to install:
 *      1) Assign EnemyMonsterPawn - SetEnemyMonsterPawn()
 *      2) Assign Mesh -  SetMesh()
 *      3) Assign MonsterLegType - SetMonsterLegType()
 *		4) Place the component above the disered location (usualy the end of the leg)
 *		5) Set SecondJointBoneName and ThirdJointBoneName in case you need it
 *		6) Set index
 *
 * How to use it:
 *		1) Setup the legs in the pawn/actor/character class (see Steps to install)
 *		2) In animBP use GetCurrentPosition() for each leg and use it as IK position for your leg bone (You can use PowerIK plugin for that or the standart IK node provided in animBP)
 *
 * There are different modifiers to prevent the leg from clipping through other meshses. 
 * See bShouldRaycastJointsWhileMoving, bActivateFirstJointRaycast and bActivateSecondJointRaycast
 *
 *
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UMonsterLegComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UMonsterLegComponent();

	// -----------------------------------------------------------------------------------------------------------
	// General
	// -----------------------------------------------------------------------------------------------------------

	/**
	 * Reference to the enemy monster pawn
	 * Assign it after initialize the component
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Leg")
	class AEnemyMonsterPawn* EnemyMonsterPawn;

	/**
	 * Reference to the mesh of the owner pawn
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Mesh")
	class USkeletalMeshComponent* Mesh;

	/**
	 * Tick component, duh
	 * Don't forget to tick the timeline here hehehe
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	/** Begin play duh */
	virtual void BeginPlay() override;


	/**
	 * Index of the leg
	 * It has 6 legs, so the index are:
	 * LEFT    3-----------------------------0    RIGHT
	 * 		   |                             |
	 * 		   |                             |
	 * 		   |                             |
	 * 		   4-----------Monster-----------1
	 * 		   |                             |
	 * 		   |                             |
	 * 		   |                             |
	 *	 	   5-----------------------------2
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg")
	int32 LegIndex;

	// -----------------------------------------------------------------------------------------------------------
	// Step
	// -----------------------------------------------------------------------------------------------------------


	/**
	 * Main method to raycast for the RaycastPosition
	 * Executes each tick
	 * Doesn't raycast if the leg is already moving(bMoving == true) or the leg shouldn't move(bCanMove == false)
	 * If the raycast is succesfull, invokes StartMovingLeg()
	 */
	UFUNCTION(BlueprintCallable)
	void RaycastLeg();

	/**
	 * Starts timeline of the step
	 * Invokes this method when the CurrentPosition and the RaycastPosition are far enough
	 */
	UFUNCTION(BlueprintCallable)
	void StartMovingLeg(FVector HitLocation);

	/**
	 * Stops the step
	 * Resets all the step params to get them ready for the next step
	 * Invokes in StartMovingLeg() whith the delay of StepTime
	 */
	UFUNCTION(BlueprintCallable, Category = "Leg")
	void StopMoving();


	/**
	 * Current position of the leg
	 * Gets updated only during moving - MoveLeg()
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Leg")
	FVector CurrentPosition;


	/**
	 * The radius of any raycast to find the FinishPosition
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg")
	float RaycastSphereRadius;


	/**
	 * When the raycast was succesful fires this method
	 * Updated CurrentPosition of the leg based on the timer of StepTime
	 */
	UFUNCTION(BlueprintCallable, Category = "Leg")
	void MoveLeg(float DeltaTime);

	/**
	 * Main method to calculate the current location
	 * How it works:
	 *		1) Interpolates X and Y values between StartPosition and FinishPosition based on a RangedMapped value of CurrentStepTime between 0 and StepTime
	 *		2) It works simillar with Z, but it adds a HighestPoint so the leg makes a parabola like motion
	 * 
	 * @return Interpolated position for CurrentStepTime
	 */
	UFUNCTION(BlueprintCallable)
	FVector GetCurrentPositionOfStep();


	/**
	 * After succefuly raycasting for the position, checks if there is something blocking between two joints
	 */
	UFUNCTION(BlueprintCallable)
	FVector Raycast(const FVector& StartPos,const FVector& EndPos,
						 FHitResult& HitResult);

	// -----------------------------------------------------------------------------------------------------------
	// Obstacle
	// -----------------------------------------------------------------------------------------------------------


	/**
	 * If while the leg was moving, it found an obstacle, we will lerp the FinishPosition with that obstacle
	 * This is the alpha value of this lerp
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg|Obstacle", meta = (UIMin = "0.0", UIMax = "1.0", EditCondition="bShouldRaycastAllJointsWhileMoving"))
	float LerpAlphaValueMovingLegWhenThereIsAnObstacle;

	/**
	 * In case there was a hit, lerps FinishPosition with this Position in MoveLeg()
	 * 
	 * @warning bShouldRaycastAllJointsWhileMoving should be true in order to work with this variable
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg|Obstacle", meta=(EditCondition="bShouldRaycastAllJointsWhileMoving"))
	FVector ObstacleHitPosition;

	/**
	 * If there is an obstacle, lerps the ObstacleHitPosition with FinishPosition
	 * changes in MoveLeg()
	 * changes in StopLeg()
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg|Obstacle", meta=(EditCondition="bShouldRaycastAllJointsWhileMoving"))
	bool bIsThereAnObstacle;


	/**
	 * Indicates if we should raycasts highest location
	 * Before starting moving the leg, checks if there is an obstacle between current position and the middle location of the movement(Highest Location)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg|Obstacle")
	bool bRaycastToTheHighestStep;

	/*
	 * Should we activate joint raycast for all joints?
	 * Joints from JointRaycastArray
	 * Executing before normal raycast!
	 *
	 * @see JointRaycastArray
	 * @see RaycastJointArray()
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Obstacle")
	bool bShouldRaycastAllJoints;


	/**
	 * Should we activate joint raycast for all joints WHILE the leg is already moving?
	 * Joints from JointRaycastArray
	 *
	 * @warning will change FinishPosition!
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Obstacle")
	bool bShouldRaycastAllJointsWhileMoving;

	/**
	 * Joints to raycast
	 * The order is important!
	 * The names of the BONES
	 *
	 * @see bShouldRaycastAllJoints
	 * @see bShouldRaycastAllJointsWhileMoving
	 * @see RaycastJointArray()
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Obstacle", meta=(EditCondition="bShouldRaycastAllJoints || bShouldRaycastAllJointsWhileMoving"))
	TArray<FName> JointRaycastArray;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Obstacle|Directional")
	bool bShouldRaycastForDirectionalObstacle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Obstacle|Directional", meta=(EditCondition="bShoudlRaycastToTheMiddleDirection"))
	float RaycastDirectionalVectorDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Obstacle|Directional")
	float AddedHightForRaycastForDirectionalObstacle;


private:


	/**
	 * Method thar calls RaycastLeg() with params, and if the hit was successful, then invokes StartMovingLeg(RaycastPosition)
	 *
	 * @param StartPos - start position from which the raycast should begin
	 * @param EndPos - end position to whcih the raycast should end
	 * @return true if there was a hit and the leg will start move, false in other cases
	 */
	bool RaycastWithStartMoving(const FVector& StartPos,const FVector& EndPos);


	/**
	 * Raycast all the joints from JointRaycastArray in order while the leg is MOVING - bMoving = true (inside of MoveLeg())
	 * If there is a blocking object between them, stores the position of the hit in ObstacleHitPosition
	 * If the hit was already registered(bIsThereAnObstacle) then it just lerps FinishPosition with ObstacleHitPosition with LerpAlphaValueMovingLegWhenThereIsAnObstacle as a lerp alpha value
	 * If there was a hit on the earlier bones (in order from JointRaycastArray) - doesn't raycast the others
	 * 
	 * @return true if there was a hit or the hit was already registered, false in other cases
	 * @see RaycastWhileMoving()
	 * @see bIsThereAnObstacle
	 * @see ObstacleHitPosition
	 * @see LerpAlphaValueMovingLegWhenThereIsAnObstacle
	 * @see bShouldRaycastJointsWhileMoving
 	 * @warning only works with bShouldRaycastJointsWhileMoving == true
	 * @warning changes FinishPosition!!!
	 * @warning should be only working while bMoving = true
	 */
	bool RaycastAllJointsWhileMoving();


	/**
	 * Raycast from the StartPos to EndPos while the leg is moving!! - MoveLeg()
	 * If there is a hit, stores the position in ObstacleHitPosition
	 * If the hit was already registered(bIsThereAnObstacle) then it just lerps FinishPosition with ObstacleHitPosition with LerpAlphaValueMovingLegWhenThereIsAnObstacle as a lerp alpha value
	 * 
	 * @returns true if there was a hit and thus the FinishPosition has been changed or the hit was registerd and thus the FinishPosition is lerping, false in other cases
 	 * @see bIsThereAnObstacle
	 * @see ObstacleHitPosition
	 * @see LerpAlphaValueMovingLegWhenThereIsAnObstacle
	 * @warning changes FinishPosition!!!
	 * @warning should be call from MoveLeg()
	 * @warning should be only working while bMoving = true
	 */
	bool RaycastWhileMoving(const FVector& StartPos, const FVector& EndPos);

	/**
	 * Indicates if we should raycasts highest location
	 * Before starting moving the leg, checks if there is an obstacle between current position and the middle location of the movement(Highest Location)
	 * After reaching the highest step, raycasts from the highest step to the finish position
	 * 
	 * Invokes in RaycastLeg()
	 *
	 * @param StartPos - starting position, should be CurrentPosition
	 * @param EndPos - finish position, should be the result of the normal raycast
	 * @return true if there was an obstacle and FinishPosition was updated, false in other cases
 	 * @see bRaycastToTheHighestStep
	 * @warning Changes FinishPosition!
	 * @warning only works when bRaycastToTheHighestStep = true
	 */
	bool RaycastToTheHighestPosition(const FVector& StartPos, const FVector& EndPos);

	/**
	 * Raycasting every joint array
	 * The order of the array matters!
	 * Executes before normal raycast, and after RaycastSecondJoint()
	 * 
	 * Invokes in RaycastLeg()
	 *
	 * @return true if there was a hit and thus the movenet start, false if there was no hit
	 * @see JointRaycastArray
	 * @see bShouldRaycastAllJoints
	 * @warning only works with bShouldRaycastAllJoints = true
	 * @warning starts moving! thus changes FinishPosition!
	 */
	bool RaycastJointArray();

	bool RaycastWhileMovingForDirectionalObstacle();
	
	FVector RaycastObstaclePositionFromTheDownVector(const FVector& GoalPosition, FHitResult& HitResult);

protected:

	// -----------------------------------------------------------------------------------------------------------
	// Step
	// -----------------------------------------------------------------------------------------------------------

	/**
	 * Defines if the leg is moving right now (Is in the timeline)
	 * If it is moving, the leg ain't raycasting
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg")
	bool bMoving;

	/**
	 * Defines if the leg should move or not
	 * Being set from EnemyMonsterPawn by SetCanMove()
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg")
	bool bCanMove;

	/**
	 * Defines if the leg has reached the highest point in the current step
	 * Enables in MoveLeg() when the leg has reached the highest point
	 * Disables in StopMoving();
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg")
	bool bHasReachedHighestPoint;

	/**
	 * Calculates the highest point in the step
	 * Gets the maxium between the Z value of StartPostion and FinishPosition and adds AddedHighStep
	 */
	float HighestPoint;

	/**
	 * The value that is added to HighestPoint
	 * Gets from EnemyMonsterPawn
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg")
	float AddedHightStep;

	/**
	 * The lerp value to interpolate CurrentPosition with the updated calculated CurrentPosition in MoveLeg()
	 * Gets from EnemyMonsterPawn
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg")
	float LerpValue;

	/**
	 * The distance needed to start a step between CurrentPosition and RaycastPosition
	 * Gets from EnemyMonsterPawn
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg", meta=(EditCondition="bShouldCheckForTheDistanceBetweenLegsToMove"))
	float DistanceBetweenLegsToMove;

	/**
	 * Should we check for the distance to start moving each leg in particular?
	 * If not, then when the other legs have moved(opposite indexes), start movign all of the currnet legs, no matter the distance
	 * See bCanMove
	 *
	 * By default we are checkign for the distance
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg")
	bool bShouldCheckForTheDistanceBetweenLegsToMove;

	/**
	 * Down length of the vector of the raycast from RaycastDownComponent
	 * Gets from EnemyMonsterPawn
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg")
	float RaycastDownLength;

	/**
	 * Time it takes to make a step
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg")
	float StepTime;

	/**
	 * Current time at the step
	 * Updates at MoveLeg()
	 * Resets at StopMoving()
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg")
	float CurrentStepTime;

	/**
	 * Start position of the step (CurrentPosition)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg")
	FVector StartPosition;

	/**
	 * Finish position of the step (RaycastPosition)
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg")
	FVector FinishPosition;

	// -----------------------------------------------------------------------------------------------------------
	// Debug
	// -----------------------------------------------------------------------------------------------------------

	/** Enables/disables debug mode */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
	bool bDrawDebug;

	/**
	 * Raycast position to use in BP for debug purpouses.
	 * Get updated only in RaycastLeg()
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
	FVector RaycastPosition;

	/** Draws debug sphere on RaycastPoisition */
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DEBUG_DrawRaycastSphere(bool bDrawEveryThick);

	/** Draw lines between two points */
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DEBUG_DrawLineBetweenPoints(const FVector& StartLocation, const FVector& EndLocation, FColor Color, bool bDrawEveryThick);

	/** Draw debug sphere of the location */
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DEBUG_DrawSphere(const FVector& Location, FColor Color, bool bDrawEveryThick);

	/**
	 * Debug lines Thickness
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
	float DEBUG_Thickness;

	/** 
	 * The radius of drawn debug spheres
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
	float DEBUG_SphereRadius;

	/** 
	 * Life Time of debug drawings
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Debug")
	float DEBUG_LifeTime;

	
	// -----------------------------------------------------------------------------------------------------------
	// Setters
	// -----------------------------------------------------------------------------------------------------------

public:
	FORCEINLINE UFUNCTION() void SetEnemyMonsterPawn(class AEnemyMonsterPawn* Pawn) { EnemyMonsterPawn = Pawn; }
	FORCEINLINE UFUNCTION() void SetMesh(class USkeletalMeshComponent* InMesh) { Mesh = InMesh; }
	FORCEINLINE UFUNCTION() FVector GetCurrentPosition() const { return CurrentPosition; }
	FORCEINLINE UFUNCTION() void SetCanMove(const bool CanMove) { bCanMove = CanMove; }
	FORCEINLINE UFUNCTION() void SetLegIndex(const int32 Index) { LegIndex = Index; }
};

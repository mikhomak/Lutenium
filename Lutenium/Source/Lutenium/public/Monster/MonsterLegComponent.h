#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MonsterLegComponent.generated.h"

/**
 * This component calculates the position for the leg in each moment
 * Main output of this component is the method GetCurrentPosition(), which returns the position where the leg is supposted to be
 * This method is called in EnemyMonsterPawn and then is transefered to the animation blueprint, where the bone of the moves to that location
 * How it works:
 *      1) Raycast down from the specific component location
 *      2) Checks if the distance between raycast position and current leg position(CurrentPosition) is greater than a distance for the step (DistanceBetweenLegsToMove)
 *      3) If it is, then starts a timeline which moves the CurrentPosition untill it reaches the raycast position
 *      4) Invokes LegHasMovedEventCaller of EnemyMonsterPawn to toggle what legs should move next.
 * Steps to install:
 *      1) Assign EnemyMonsterPawn - SetEnemyMonsterPawn()
 *      2) Assign Mesh -  SetMesh()
 *      3) Assign MonsterLegType - SetMonsterLegType()
 *		4) Set SecondJointSocketName and ThirdJointSocketName in case you need it
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
	 * When the leg is moving (MoveLeg()) - should we check each frame if there is an obstacle between SecondJointSocketName and FinishPosition ?
	 * If so, set this obstacle position (Hit position) as a FinishPosition
	 *
	 * @warning uses bActivateSecondJointRaycast
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg|Obstacle", meta=(EditCondition="bShouldRaycastJointsWhileMoving"))
	bool bShouldRaycastJointsWhileMoving;

	/**
	 * If while the leg was moving, it found an obstacle, we will lerp the FinishPosition with that obstacle
	 * This is the alpha value of this lerp
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg|Obstacle", meta = (UIMin = "0.0", UIMax = "1.0", EditCondition="bShouldRaycastJointsWhileMoving"))
	float LerpAlphaValueMovingLegWhenThereIsAnObstacle;

	/**
	 * In case there was a hit, lerps FinishPosition with this Position in MoveLeg()
	 * 
	 * @warning bShouldRaycastJointsWhileMoving should be true in order to work with this variable
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg|Obstacle", meta=(EditCondition="bShouldRaycastJointsWhileMoving"))
	FVector ObstacleHitPosition;

	/**
	 * If there is an obstacle, lerps the ObstacleHitPosition with FinishPosition
	 * changes in MoveLeg()
	 * changes in StopLeg()
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg|Obstacle", meta=(EditCondition="bShouldRaycastJointsWhileMoving"))
	bool bIsThereAnObstacle;

	// -----------------------------------------------------------------------------------------------------------
	// Joint Sockets
	// -----------------------------------------------------------------------------------------------------------

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

	/**
	 * Before doing normal raycast from the component position
	 * Doing the raycast from monster location(BodySocket) to first socket (Has to be on the top of the leg)
	 * If there was a hit, place the CurrentPosition to that hit
	 * This prevents leg from clipping through buildings
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg|Joint Raycast")
	bool bActivateFirstJointRaycast;

	/**
	 * Before doing normal raycast from the component position
	 * Doing the raycast from first socket to third socket (has to be on the end of the leg)
	 * If there was a hit, place the CurrentPosition to that hit
	 * This prevents leg from clipping through buildings
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Leg|Joint Raycast")
	bool bActivateSecondJointRaycast;

	/**
	 * Second Joint Socket
	 * Has to be on the highest point of the leg
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Joint Raycast", meta=(EditCondition="bActivateFirstJointRaycast"))
	FName SecondJointSocketName;

	/**
	 * Maxium attempts of the raycast between joints
	 * When CurrentAttempsAtSecondJointRaycast > MaxAttempsAtSecondJointRaycast, bActivateFirstJointRaycast will be ignored and the raycast between Mesh and First joint wouldn't happen
	 * It prevents from stucking the mesh
	 * So the next step this raycat wouldn't be considered
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Joint Raycast", meta=(EditCondition="bActivateFirstJointRaycast"))
	int32 MaxAttempsAtSecondJointRaycast;
	
	/**
	 * Current attempts for first joint raycast before starting to ignore it
	 * When CurrentAttempsAtSecondJointRaycast > MaxAttempsAtSecondJointRaycast, bActivateFirstJointRaycast will be ignored and the raycast between Mesh and First joint wouldn't happen
	 * It prevents from stucking the mesh
	 *
	 * Updates in RaycastFirstJoint()
	 * Resest in RaycastFirstJoint() when the CurrentAttempsAtSecondJointRaycast > MaxAttempsAtSecondJointRaycast
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Joint Raycast", meta=(EditCondition="bActivateFirstJointRaycast"))
	int32 CurrentAttempsAtSecondJointRaycast;

	/**
	 * Second Joint Socket
	 * Has to be on the lowest(the end of the leg) point of the leg
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Joint Raycast", meta=(EditCondition="bActivateSecondJointRaycast"))
	FName ThirdJointSocketName;

	/**
	 * Maxium attempts of the raycast between joints
	 * When CurrentAttempsAtThirdJointRaycast > MaxAttempsAtThirdJointRaycast, bActivateSecondJointRaycast will be ignored and the raycast between Mesh and First joint wouldn't happen
	 * It prevents from stucking the mesh
	 * So the next step this raycat wouldn't be considered
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Joint Raycast", meta=(EditCondition="bActivateSecondJointRaycast"))
	int32 MaxAttempsAtThirdJointRaycast;

	/**
	 * Current attempts for first joint raycast before starting to ignore it
	 * When CurrentAttempsAtThirdJointRaycast > MaxAttempsAtThirdJointRaycast, bActivateSecondJointRaycast will be ignored and the raycast between Mesh and First joint wouldn't happen
	 * It prevents from stucking the mesh
	 *
	 * Updates in RaycastSecondJoint()
	 * Resest in RaycastSecondJoint() when the CurrentAttempsAtThirdJointRaycast > MaxAttempsAtThirdJointRaycast
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Joint Raycast", meta=(EditCondition="bActivateSecondJointRaycast"))
	int32 CurrentAttempsAtThirdJointRaycast;


private:
	/**
	 * Raycasts the first joint
	 * Invokes in RaycastLeg()
	 * 
	 * @warning works only when bActivateFirstJointRaycast == true
	 * @see bActivateFirstJointRaycast
	 * @see SecondJointSocketName
	 * @return true if the raycast was successful, false in other case or bActivateFirstJointRaycast == false
	 */
	bool RaycastFirstJoint();

	/**
	 * Raycasts the second joint
	 * Invokes in RaycastLeg()
	 *
	 * @warning works only when bActivateSecondJointRaycast == true
	 * @see bActivateSecondJointRaycast
	 * @see ThirdJointSocketName
	 * @return true if the raycast was successful, false in other case or bActivateSecondJointRaycast == false
	 */
	bool RaycastSecondJoint();

	/**
	 * Method thar calls RaycastLeg() with params, and if the hit was successful, then invokes StartMovingLeg(RaycastPosition)
	 *
	 * @param StartPos - start position from which the raycast should begin
	 * @param EndPos - end position to whcih the raycast should end
	 * @return true if there was a hit and the leg will start move, false in other cases
	 */
	bool RaycastWithStartMoving(const FVector& StartPos,const FVector& EndPos);


	/**
	 * Raycast inside of the MoveLeg() method
	 * Raycast from second to the third sockets: SecondJointSocketName ----> ThirdJointSocketName
	 * If there is a blocking object between them, stores the position of the hit in ObstacleHitPosition
	 * If the hit was already registered(bIsThereAnObstacle) then it just lerps FinishPosition with ObstacleHitPosition with LerpAlphaValueMovingLegWhenThereIsAnObstacle as a lerp alpha value
	 *
	 * @warning only works with bShouldRaycastJointsWhileMoving == true
	 * @warning changes FinishPosition!!!
	 * @see bIsThereAnObstacle
	 * @see ObstacleHitPosition
	 * @see LerpAlphaValueMovingLegWhenThereIsAnObstacle
	 * @see bShouldRaycastJointsWhileMoving
	 */
	void RaycastSecondJointsWhileMoving();

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg")
	float DistanceBetweenLegsToMove;

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
	void DEBUG_DrawRaycastSphere();

	/** Draw lines between two points */
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DEBUG_DrawLineBetweenPoints(const FVector& StartLocation, const FVector& EndLocation, FColor Color);

	/** Draw debug sphere of the location */
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DEBUG_DrawSphere(const FVector& Location, FColor Color);

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

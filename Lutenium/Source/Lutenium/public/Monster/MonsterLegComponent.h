#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterLegComponent.generated.h"

/**
 * This component calculates the position for the leg in each moment
 * Main output of this component is the method GetCurrentPosition(), which returns the position where the leg is supposted to be
 * This method is called in EnemyMonsterPawn and then is transefered to the animation blueprint, where the bone of the moves to that location
 * How it works:
 *      1) Raycast down from the specific socket on the monster mesh - RaycastSocket. This socket moves with the body, not with the leg!
 *      2) Checks if the distance between raycast position and current leg position(CurrentPosition) is greater than a distance for the step (DistanceBetweenLegsToMove)
 *      3) If it is, then starts a timeline which moves the CurrentPosition untill it reaches the raycast position
 *      4) Invokes LegHasMovedEventCaller of EnemyMonsterPawn to toggle what legs should move next.
 * Steps to install:
 *      1) Assign EnemyMonsterPawn - SetEnemyMonsterPawn()
 *      2) Assign MonsterMesh -  SetMonsterMesh()
 *      3) Assign MonsterLegType - SetMonsterLegType()
 *		4) Put the name of the socket in RaycastSocket
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UMonsterLegComponent : public UActorComponent
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
	 * Reference to the monster mesh
	 * Assign it after initialize the component
	 */
	UPROPERTY(BlueprintReadWrite, Category = "Leg")
	class USkeletalMeshComponent* MonsterMesh;

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
	 * Main socket to update RaycastPosition
	 * @warning This socket should be attached to the mesh, not the leg!
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg")
	FName RaycastSocket;


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
	 * @return Interpolated position for CurrentStepTime
	 */
	UFUNCTION(BlueprintCallable)
	FVector GetCurrentPositionOfStep();


	// -----------------------------------------------------------------------------------------------------------
	// Joint raycast
	// -----------------------------------------------------------------------------------------------------------

	/**
	 * After main raycast to find RayCastPosition, makes another raycast to find if there is something blocking from this socket to SecondJointSocket
	 * If there is a blocking hit, sets RaycastPosition to this hit location
	 * It was made to prevent clipping the leg through objects
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Joint")
	FName FirstJointSocket;

	/**
	 * Second joint to raycast from the FirstJointSocket
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Leg|Joint")
	FName SecondJointSocket;


	/**
	 * After succefuly raycasting for the position, checks if there is something blocking between two joints
	 */
	FVector Raycast(FVector& StartPos, FVector& EndPos,
						 FHitResult& HitResult, FCollisionQueryParams& CollisionParams);

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
	 * Down length of the vector of the raycast from RaycastSocket
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
	void DEBUG_DrawLineBetweenPoints(FVector& StartLocation, FVector& EndLocation);

	// -----------------------------------------------------------------------------------------------------------
	// Setters
	// -----------------------------------------------------------------------------------------------------------

public:
	FORCEINLINE UFUNCTION() void SetMonsterMesh(class USkeletalMeshComponent* Mesh) { MonsterMesh = Mesh; }
	FORCEINLINE UFUNCTION() void SetEnemyMonsterPawn(class AEnemyMonsterPawn* Pawn) { EnemyMonsterPawn = Pawn; }
	FORCEINLINE UFUNCTION() FVector GetCurrentPosition() const { return CurrentPosition; }
	FORCEINLINE UFUNCTION() void SetCanMove(const bool CanMove) { bCanMove = CanMove; }
	FORCEINLINE UFUNCTION() void SetLegIndex(const int32 Index) { LegIndex = Index; }
};

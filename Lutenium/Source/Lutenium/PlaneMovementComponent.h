// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlaneMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UPlaneMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	/** StaticMesh component that will be the visuals for our flying pawn */
	UPROPERTY(Category = Player, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class APlayerPawn* PlayerPawn;


public:	
	// Sets default values for this component's properties
	UPlaneMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Bound to the thrust axis */
	void ThrustInput(float Val);

	/** Bound to the vertical axis */
	void PitchInput(float Val);

	/** Bound to the horizontal axis */
	void YawnInput(float Val);

	void RollInput(float Val);


private: 
	/** How quickly forward speed changes */
	UPROPERTY(Category = Speed, EditAnywhere)
		float ThrustAcceleration;

	/** Max forward speed */
	UPROPERTY(Category = Speed, EditAnywhere)
		float ThrustMaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Speed, EditAnywhere)
		float ThrustMinSpeed;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Speed, EditAnywhere)
		float YawnSpeed;

	UPROPERTY(Category = Speed, EditAnywhere)
		float RollSpeed;

	UPROPERTY(Category = Speed, EditAnywhere)
		float PitchSpeed;


	/** Current forward speed */
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;
};

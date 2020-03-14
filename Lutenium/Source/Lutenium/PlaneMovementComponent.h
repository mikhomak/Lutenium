// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlaneMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UPlaneMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(Category = Player, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class APlayerPawn* PlayerPawn;



public:	
	UPlaneMovementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ThrustInput(float Val);

	void PitchInput(float Val);

	void YawnInput(float Val);

	void RollInput(float Val);


private: 
	class UStaticMeshComponent* PlayerMesh;

	UPROPERTY(Category = Control, EditAnywhere)
		float AirControl;

	UPROPERTY(Category = Speed, EditAnywhere)
		float ThrustAcceleration;

	UPROPERTY(Category = Speed, EditAnywhere)
		float ThrustMaxSpeed;

	UPROPERTY(Category = Speed, EditAnywhere)
		float ThrustMinSpeed;

	float CurrentThrust;

	bool ThrustUp;	
	
	bool ThrustDown;

	void AddTorqueToThePlane(FVector Direction, float InputVal);
	
	void Thrusting(float InputVal);
	void AddThrust();
};
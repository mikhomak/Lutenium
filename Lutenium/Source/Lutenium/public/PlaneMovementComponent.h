// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlaneMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UPlaneMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(Category = Player, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class APlayerPawn* PlayerPawn;

    UPROPERTY(Category = Mesh, EditAnywhere)
    class USkeletalMeshComponent* PlayerMesh;

public:
	UPlaneMovementComponent();

protected:
	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	void ThrustInput(float Val);

	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	void PitchInput(float Val);

	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	void YawnInput(float Val);

	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	void RollInput(float Val);

	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	void DashForward();

	UFUNCTION(BlueprintCallable, Category = "Pawn", meta = (AdvancedDisplay = "2"))
	void SetMesh(USkeletalMeshComponent* Mesh);

	UFUNCTION(BlueprintCallable, Category = "Pawn", meta = (AdvancedDisplay = "2"))
	void SetPawn(APlayerPawn* Pawn);

	UPROPERTY(Category = "Dash", EditAnywhere)
	float MaxDashes;

private:

	UPROPERTY(Category = Control, EditAnywhere)
	float AirControl;

	UPROPERTY(Category = Control, EditAnywhere)
	float YawnControl;

	UPROPERTY(Category = Control, EditAnywhere)
	float PitchControl;

	UPROPERTY(Category = Control, EditAnywhere)
	float RollControl;

	UPROPERTY(Category = Control, EditAnywhere)
	float AerodynamicMultiplier;

	UPROPERTY(Category = Speed, EditAnywhere)
	float MaxThrustUpAcceleration;

	UPROPERTY(Category = Speed, EditAnywhere)
	float ThrustMaxSpeed;

	UPROPERTY(Category = Speed, EditAnywhere)
	float ThrustMinSpeed;

	UPROPERTY(Category = "Dash", EditAnywhere)
	float DashImpact;

	UPROPERTY(Category = Speed, EditAnywhere)
	float CurrentAcceleration;

	UPROPERTY(Category = Speed, EditAnywhere)
	float ThrustUpAcceleration;

	UPROPERTY(Category = Speed, EditAnywhere)
	float ThrustDownAcceleration;

	UPROPERTY(Category = CustomPhysics, EditAnywhere)
	float CustomGravity;

	float CurrentThrust;

	bool ThrustUp;

	bool bThrusting;

	float MaxThrustDownAcceleration;

	

	float Dot;

	float DashesLeft;

	void AddTorqueToThePlane(FVector Direction, float InputVal) const;

	void Thrusting(float InputVal);

	void AddThrust(float DeltaTime) const;

	void CalculateAcceleration();

	void AddGravityForce(float DeltaTime) const;

    void CalculateAerodynamic(float DeltaTime);

};

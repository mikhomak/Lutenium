// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MissileTargetHit.h"
#include "PlayerPawn.generated.h"


UCLASS(Config=Game)
class APlayerPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlaneMesh;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(Category = Movement, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPlaneMovementComponent* PlaneMovement;

	UPROPERTY(Category = Movement, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* PlaneBox;

public:
	APlayerPawn();
	virtual void Tick(float DeltaSeconds) override;
	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	float GetYawnInput() const;

	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	float GetRollInput() const;

	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	float GetPitchInput() const;

	UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
	float GetThrustInput() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "PlaneMovement")
	void DotHasChange();

	UFUNCTION(BlueprintImplementableEvent, Category = "PlaneMovement")
	void DashImpact();

	// ------------------------------------------------------------------
	// Missile
	// ------------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	FVector MissileOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	class USceneComponent* MissileTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	FVector MissileTargetRaycastHitLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	EMissileTargetHit MissileTargetRaycastHitType;

	UPROPERTY(EditDefaultsOnly, Category = Missile)
	TSubclassOf<class AMissile> MissileClass;

	UPROPERTY(EditDefaultsOnly, Category = Missile)
	float MissileTraceLength;

	UPROPERTY(EditDefaultsOnly, Category = Missile)
	float FirstRaytraceRadius;

	UPROPERTY(EditDefaultsOnly, Category = Missile)
	float SecondRaytraceRadius;

protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

private:
	UInputComponent* InputComponent;

	UFUNCTION()
	void FireMissile();

public:
	FORCEINLINE class UStaticMeshComponent* GetPlaneMesh() const { return PlaneMesh; }
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE class UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE class UPlaneMovementComponent* GetPlaneMovement() const { return PlaneMovement; }
	FORCEINLINE class UBoxComponent* GetPlaneBox() const { return PlaneBox; }
};

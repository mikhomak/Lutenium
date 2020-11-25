// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Missile.generated.h"

UCLASS()
class LUTENIUM_API AMissile : public AActor
{
	GENERATED_BODY()

public:
	AMissile();

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Missile)
	class USphereComponent* SphereComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Movement)
    class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION(BlueprintCallable)
	void ThrowMissile(FVector ThrownDirection, float ForceAmount);

	UFUNCTION()
	void SetTargetOrDirection(class USceneComponent* Target, const FVector& ShootDirection);

	FORCEINLINE void SetParentPawn(class APlayerPawn* Pawn){ PlayerPawn = Pawn;}


protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* MissileMesh;

	UPROPERTY(BlueprintReadOnly)
	class APlayerPawn* PlayerPawn;

	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	FVector Direction;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Damage")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Damage")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly)
	float MissileLifeSpan;

	UFUNCTION(BlueprintCallable, Category="Damage")
	void Explode();

	UFUNCTION(BlueprintImplementableEvent, Category = "Damage")
	void OnExplode();
	// ------------------------------------------------------------------
	// Defected
	// ------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category="Defected")
	float DistanceToThePlayerWhenTheDefectedMissileIsAboutToBlowUp;

	UPROPERTY(EditDefaultsOnly, Category="Defected")
	struct FTimeline DefectedTimeline;

	UPROPERTY(EditDefaultsOnly, Category="Defected")
	float DefecteedGravityForceAmount;

	UPROPERTY(EditDefaultsOnly, Category="Defected")
	float DefecteedImpulseForceAmount;

    UPROPERTY(EditAnywhere, Category="Defected")
    class UCurveFloat* Curve;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Defected")
	bool bIsDefected;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Defected")
	bool bIsPawnGravited;

	UFUNCTION()
	void DefectedMissileGravity();

	UFUNCTION()
	void DefectedMissileImpulse();

	UFUNCTION(BlueprintImplementableEvent, Category = "Defected")
	void GotDefected();

	UFUNCTION(BlueprintImplementableEvent, Category = "Defected")
	void DefectedImpulse();
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.generated.h"

UCLASS()
class LUTENIUM_API AMissile : public AActor
{
	GENERATED_BODY()

public:
	AMissile();

protected:
	virtual void BeginPlay() override;



	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* MissileMesh;

	class APawn* ParentPawn;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Missile)
	class USphereComponent* SphereComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Movement)
    class UProjectileMovementComponent* ProjectileMovement;


	UPROPERTY(EditAnywhere)
	FVector Direction;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly)
	float WavesLifeSpan;

	UFUNCTION(BlueprintCallable)
	void ThrowMissile(FVector ThrownDirection, float ForceAmount);

	UFUNCTION()
	void SetTargetOrDirection(USceneComponent* Target, const FVector& ShootDirection);

	FORCEINLINE void SetParentPawn(APawn* Pawn){ ParentPawn = Pawn;}


};


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
	
	UPROPERTY(VisibleDefaultsOnly, Category = Missile)
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
    class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* MeshComp;

	class APawn* ParentPawn;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditDefaultsOnly)
	float InitialSpeed;

	UPROPERTY(EditDefaultsOnly)
	float MaxSpeed;

	UPROPERTY(EditDefaultsOnly)
	float ExplosionRadius;

	UPROPERTY(EditAnywhere)
	FVector Direction;

	UPROPERTY(EditDefaultsOnly)
	float Damage;

	UPROPERTY(EditDefaultsOnly)
	float TimeBeforeFly;
	
	UFUNCTION()
	void SetTargetOrDirection(USceneComponent* Target, const FVector& ShootDirection);

	UFUNCTION(BlueprintNativeEvent)
	void AfterInstantiate();

	UFUNCTION(BlueprintNativeEvent)
    void BeginFlying();

	FORCEINLINE void SetParentPawn(APawn* Pawn){ ParentPawn = Pawn;}

private:
	void StartFlying();

};


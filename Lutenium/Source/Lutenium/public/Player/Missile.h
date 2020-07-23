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

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
    void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly, Category = Missile)
	class UCapsuleComponent* CapsuleCollider;

	UPROPERTY(VisibleAnywhere, Category = Movement)
    class UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere)
	float InitialSpeed;

	UPROPERTY(EditAnywhere)
	float MaxSpeed;

	UPROPERTY(EditAnywhere)
	FVector Direction;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float TimeBeforeFly;
	
	UFUNCTION(BlueprintCallable)
	void SetDirection(const FVector& ShootDirection);

	UFUNCTION(BlueprintNativeEvent)
	void AfterInstantiate();

	UFUNCTION(BlueprintNativeEvent)
    void BeginFlying();
	
private:
	void StartFlying();
};


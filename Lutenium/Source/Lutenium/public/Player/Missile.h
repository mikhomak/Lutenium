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


	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MissileMesh;
	
	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	FVector Direction;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	APawn* Pawn;

	UPROPERTY(EditAnywhere)
	float TimeBeforeFly;
	
	UFUNCTION(BlueprintCallable)
	void SetPawn(APawn* NewPawn);

	UFUNCTION(BlueprintNativeEvent)
	void AfterInstantiate();

	UFUNCTION(BlueprintNativeEvent)
    void BeginFlying();
	
private:

	bool bFlying;
	
	void StartFlying();
};


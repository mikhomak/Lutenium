// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Scream.generated.h"


UCLASS( )
class LUTENIUM_API AScream : public AActor
{
	GENERATED_BODY()

public:
	AScream();



	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	class USphereComponent* MainSphereComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Mesh)
	class UStaticMeshComponent* FirstWaveMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Mesh)
	class UStaticMeshComponent* SecondWaveMesh;

	UPROPERTY(EditDefaultsOnly, Category=Damage)
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category=Damage)
	float ExpansionSpeed;

	UFUNCTION(BlueprintCallable)
	void  FirstWaveOverlap(
		class UPrimitiveComponent *OverlappedComp,
		class AActor *OtherActor,
		class UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult);

	UFUNCTION(BlueprintCallable)
	void  SecondWaveOverlap(
		class UPrimitiveComponent *OverlappedComp,
		class AActor *OtherActor,
		class UPrimitiveComponent *OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult &SweepResult
	);

	UPROPERTY(EditDefaultsOnly, Category="Waves")
	float ScalingSpeed;

	UPROPERTY(EditDefaultsOnly, Category="Waves")
	float SecondFaveDelay;

	UPROPERTY(EditDefaultsOnly, Category="Waves")
	float WavesLifeSpan;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetDamage(float& NewDamage) { Damage = NewDamage;}
	FORCEINLINE void SetExpansionSpeed(float& NewExpansionSpeed) { ExpansionSpeed = NewExpansionSpeed;}

};

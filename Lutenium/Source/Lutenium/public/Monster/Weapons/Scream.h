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
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category=Damage)
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category=Damage)
	float ExpansionSpeed;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetDamage(float& NewDamage) { Damage = NewDamage;}
	FORCEINLINE void SetExpansionSpeed(float& NewExpansionSpeed) { ExpansionSpeed = NewExpansionSpeed;}

};

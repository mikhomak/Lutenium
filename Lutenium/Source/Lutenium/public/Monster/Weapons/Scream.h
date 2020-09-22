// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Scream.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UScream : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScream();


	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category=Damage)
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category=Damage)
	float ExpansionSpeed;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetDamage(float& NewDamage) { Damage = NewDamage;}
	FORCEINLINE void SetExpansionSpeed(float& NewExpansionSpeed) { ExpansionSpeed = NewExpansionSpeed;}
	
};

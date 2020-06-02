// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingBeam.generated.h"

UCLASS()
class LUTENIUM_API ABuildingBeam : public AActor
{
	GENERATED_BODY()



public:	
	ABuildingBeam();

	UPROPERTY(Category = Mesh, BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PlatformStart;

	UPROPERTY(Category = Mesh, BlueprintReadWrite, EditAnywhere, meta = (MakeEditWidget = ""))
	class UStaticMeshComponent* PlatformEnd;


	UPROPERTY(Category = Mesh, BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BeamMesh;

	UFUNCTION(BlueprintCallable, Category = "Scales", meta = (AdvancedDisplay = "2"))
	FVector2D GetScale() const;

	UFUNCTION(BlueprintCallable, Category = "Scales", meta = (AdvancedDisplay = "2"))
	float GetRollRotation() const;

	UFUNCTION(BlueprintCallable, Category = "Scales", meta = (AdvancedDisplay = "2"))
	TArray<FVector> CalculateSplinePoints() const;


	UPROPERTY(Category = Scales, EditAnywhere)
	FVector2D Scale;

	UPROPERTY(Category = Scales, EditAnywhere)
	float RollRotation;

	UPROPERTY(Category = Scales, EditAnywhere)
	int Points;
};

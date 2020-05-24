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
	class USplineComponent* Beam;

	UPROPERTY(Category = Mesh, BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BeamMesh;

private:
	UFUNCTION(BlueprintCallable, Category = "Geometry", meta = (AdvancedDisplay = "2"))
	FVector FindMidPoint(FVector& Start, FVector& End);
};

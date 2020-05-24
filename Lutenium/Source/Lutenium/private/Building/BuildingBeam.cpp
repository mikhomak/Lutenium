// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StaticMeshComponent.h"
#include "../../public/Building/BuildingBeam.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ABuildingBeam::ABuildingBeam() {
	

	
	PlatformStart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformStart"));
	RootComponent = PlatformStart;
	PlatformEnd = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformEnd"));
	Beam = CreateDefaultSubobject<USplineComponent>(TEXT("Beam"));

}

FVector FindMidPoint(FVector& Start, FVector& End) {
	FVector Vec = (End - Start);
	float Length;
	FVector Nor;
	Vec.ToDirectionAndLength(Nor, Length);
	Length /= 2;
	return Start + Nor * Length;
}
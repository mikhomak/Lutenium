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
	Points = 2;
}

FVector2D ABuildingBeam::GetScale() {
	return Scale;
}

float ABuildingBeam::GetRollRotation() {
	return RollRotation;
}

TArray<FVector> ABuildingBeam::CalculateSplinePoints() {
	TArray<FVector> Result;
	FVector Start = GetActorLocation();
	FVector End = PlatformEnd->GetComponentLocation();
	Result.Add(Start);
	
	FVector Direction;
	float Length;
	FVector PointVector = (End - Start)/Points;
	PointVector.ToDirectionAndLength(Direction, Length);

	for (int32 i = 1; i < Points; i++) {
		FVector Point = Start + (Direction * Length * i);
		Result.Add(Point);
	}
	Result.Add(End);
	return Result;
}
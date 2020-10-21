// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Emp.generated.h"

UCLASS()
class LUTENIUM_API AEmp : public AActor
{
	GENERATED_BODY()

public:

	AEmp();

 	UPROPERTY(EditDefaultsOnly, Category=Mesh)
    class USphereComponent* MainSphereComp;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category=Mesh)
    class UStaticMeshComponent* EmpMesh;

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    FVector ScalingMultiplier;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float ScalingSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float EmpRotationForce;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float EmpLifeTime;

	UFUNCTION(BlueprintCallable)
    void EmpOverlap(
        class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;


};

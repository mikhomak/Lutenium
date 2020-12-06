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

    UPROPERTY(EditDefaultsOnly, Category="Mesh")
    class USphereComponent* MainSphereComp;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Mesh")
    class UStaticMeshComponent* WaveMesh;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    float Damage;

    UPROPERTY(EditDefaultsOnly, Category="Damage")
    float ExpansionSpeed;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Damage")
    bool bDragOrImpulse; // true - dragging the player, false - impulse the player away

    UFUNCTION(BlueprintCallable)
    void WaveOverlap(class AActor* OtherActor);

    UFUNCTION(BlueprintCallable)
    void WaveOverlapEnd(class AActor* OtherActor);

    UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
    FVector ScalingMultiplier;

    UPROPERTY(EditDefaultsOnly, Category="Waves")
    float ScalingSpeed;

    UPROPERTY(EditDefaultsOnly, Category="Waves")
    float WavesLifeSpan;

    UPROPERTY(EditDefaultsOnly, Category="Waves")
    float ForceImpact;

    UPROPERTY(EditDefaultsOnly, Category="Waves")
    float DragForce;
protected:
    virtual void BeginPlay() override;


public:
    virtual void Tick(float DeltaTime) override;

    FORCEINLINE void SetDamage(float& NewDamage) { Damage = NewDamage; }
    FORCEINLINE void SetExpansionSpeed(float& NewExpansionSpeed) { ExpansionSpeed = NewExpansionSpeed; }
    FORCEINLINE UFUNCTION(BlueprintCallable)
    void SetDragOrImpulse(bool DragOrImpulse) { bDragOrImpulse = DragOrImpulse; }
};

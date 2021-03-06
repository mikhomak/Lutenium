// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Scream.generated.h"


UCLASS( )
class LUTENIUM_API AScream : public AActor
{
    GENERATED_BODY()

public:
    AScream();


    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Mesh")
    class UStaticMeshComponent* WaveMesh;


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

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Scream")
    bool bIsEmpScream;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Scream")
    float EmpForce;



    /**
     * Changes the mesh when its an semisphere mesh
     * Replaces WaveMesh by this mesh if SetUseSecondMesh(true)
     * Being invoked in SetUseSecondMesh();
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scream")
    UStaticMesh* SemisphereScreamMesh;

    /**
     * Replaces WaveMesh by SemisphereScreamMesh if bUseSecondMesh is true
     * @param bUseSecondMesh - replace WaveMesh by SemisphereScreamMesh?
     */
    FORCEINLINE UFUNCTION(Category = "Scream")
    void SetUseSecondMesh(bool bUseSecondMesh)
    {
        if(bUseSecondMesh)
        {
            WaveMesh->SetStaticMesh(SemisphereScreamMesh);
            WaveMesh->SetMaterial(0, SemisphereMaterial);
        }
    }

    /**
     * Sets the material when the WaveMesh is being replaced by the SemisphereScreamMesh
     * @see SetUseSecondMesh();
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Scream")
    UMaterialInstance* SemisphereMaterial;

protected:
    virtual void BeginPlay() override;


public:
    virtual void Tick(float DeltaTime) override;

    FORCEINLINE void SetExpansionSpeed(float& NewExpansionSpeed) { ExpansionSpeed = NewExpansionSpeed; }
    FORCEINLINE UFUNCTION(BlueprintCallable)
    void SetDragOrImpulse(bool DragOrImpulse) { bDragOrImpulse = DragOrImpulse; }
};

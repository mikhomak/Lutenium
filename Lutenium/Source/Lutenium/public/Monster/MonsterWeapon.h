// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ObjectMacros.h"

#include "MonsterWeapon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API AMonsterWeapon : public AActor
{
    GENERATED_BODY()

public:
    AMonsterWeapon();

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Monster)
    class AEnemyMonsterPawn* MonsterPawn;

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Monster)
    class USkeletalMeshComponent* MonsterMesh;

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Monster)
    class UStaticMeshComponent* WeaponMesh;

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Monster)
    class USphereComponent* Hurtbox;

    UFUNCTION()
    virtual void OnTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                      AActor* DamageCauser);
protected:
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category=Health)
    float Health;


public:
    FORCEINLINE void SetMonsterMesh(class USkeletalMeshComponent* Mesh) { MonsterMesh = Mesh; }
    FORCEINLINE void SetMonsterPawn(class AEnemyMonsterPawn* Pawn) { MonsterPawn = Pawn; }
};

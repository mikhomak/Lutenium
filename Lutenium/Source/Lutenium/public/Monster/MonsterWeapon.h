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

    UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category=Monster)
    class AEnemyMonsterPawn* MonsterPawn;

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Monster)
    class USkeletalMeshComponent* MonsterMesh;

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Monster)
    class UStaticMeshComponent* WeaponMesh;

    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category=Monster)
    class USphereComponent* Hurtbox;

    UFUNCTION(BlueprintCallable)
    void OnTakeDamage(float Damage);

    UFUNCTION(BlueprintCallable)
    void Die();

	UFUNCTION(BlueprintImplementableEvent, Category = "Health")
    void DieEvent();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category="Health")
    float Health;

    /* Take damage & handle death */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;


public:
    FORCEINLINE void SetMonsterMesh(class USkeletalMeshComponent* Mesh) { MonsterMesh = Mesh; }
    FORCEINLINE void SetMonsterPawn(class AEnemyMonsterPawn* Pawn) { MonsterPawn = Pawn; }
};

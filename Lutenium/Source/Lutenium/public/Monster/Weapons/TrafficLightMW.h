// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponsUtils/TrafficLight.h"
#include "WeaponsUtils/TrafficLightPosition.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "TrafficLightMW.generated.h"

/**
 * Traffic light weapon
 * On the 0 level
 * Has 3 Lights: center, right and left
 * Each light have three states: red, green and yellow
 * Red - throws and defecets missile, and starts dragging player away
 * Yellow - throws missile and dragging player away
 * Green - takes damage and does nothing to the player
 */
UCLASS()
class LUTENIUM_API ATrafficLightMW : public AMonsterWeapon
{
    GENERATED_BODY()
public:

    ATrafficLightMW();

    // ------------------------------------------------------------------
    // Change light
    // ------------------------------------------------------------------

    UFUNCTION(BlueprintCallable)
    void ChangeLight(ETrafficLightPosition Position, ETrafficLight Light);

	UFUNCTION(BlueprintImplementableEvent, Category="Light")
	void OnLightChanged(const ETrafficLightPosition Position, const ETrafficLight Light);

    // ------------------------------------------------------------------
    // Lights status
    // ------------------------------------------------------------------

    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLight LeftLightStatus;

    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLight RightLightStatus;

    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLight CenterLightStatus;

    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLightPosition LatestRedLightPosition;

    FORCEINLINE UFUNCTION(Category = "Light")
    void SetLatestRedLightPosition(const ETrafficLight TrafficLightStatus, const ETrafficLightPosition TrafficLightPosition)
    {
        if(TrafficLightStatus == ETrafficLight::Red)
        {
            LatestRedLightPosition = TrafficLightPosition;
        }
    }

    // ------------------------------------------------------------------
    // Attack
    // ------------------------------------------------------------------
    virtual void ExecuteAttack() override;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attack")
    float PlayerDragForce;

    /**
     * Takes hurt box damage and changes the light status of the light that took damage
     * After changing the status, calls TakeHurtboxDamage(Damage);
     */
    UFUNCTION(BlueprintCallable)
    void TakeHurtboxDamageChangingLight(float Damage, const ETrafficLightPosition TrafficLightPosition);

    /**
     * Overriding this method to call TakeHurtboxDamageChangingLight() with the position of the hurtbox that was hit
     */
    virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;



protected:

    // ------------------------------------------------------------------
    // Missile
    // ------------------------------------------------------------------

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Missile")
    float MissileThrowForce;

	UFUNCTION(BlueprintImplementableEvent, Category="Missile")
	void OnMissileGetThrownAway(const ETrafficLightPosition Position, const ETrafficLight Light, const FVector ImpactPosition, const bool bDefected);

    // ------------------------------------------------------------------
    // Lights overlaps
    // ------------------------------------------------------------------

    UFUNCTION(BlueprintCallable)
    void LightBeginOverlap(class AActor* Actor, const ETrafficLight TrafficLightStatus, const ETrafficLightPosition TrafficLightPosition);

    UFUNCTION(BlueprintCallable)
    void LightEndOverlap(class AActor* Actor, const ETrafficLight TrafficLightStatus, const ETrafficLightPosition TrafficLightPosition);

    // ------------------------------------------------------------------
    // Lights Meshes
    // ------------------------------------------------------------------

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UStaticMeshComponent* LeftLightMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UStaticMeshComponent* RightLightMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UStaticMeshComponent* CenterLightMesh;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Weapon")
    class USphereComponent* HurtboxRight;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Weapon")
    class USphereComponent* HurtboxLeft;
    // ------------------------------------------------------------------
    // General
    // ------------------------------------------------------------------
	virtual void BeginPlay() override;

    /** Before attack event */
    virtual void BeforeAttackEvent_Implementation() override;
};

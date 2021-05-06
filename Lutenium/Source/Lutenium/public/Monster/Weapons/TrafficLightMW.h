// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponsUtils/TrafficLight.h"
#include "WeaponsUtils/TrafficLightPosition.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "TrafficLightMW.generated.h"

/**
 * Traffic light weapon
 * On the lowest level
 * Has 3 Lights: center, right and left
 * Each light have three states: red, green and yellow
 * Red - throws and defecets missile, emps the player
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

    /**
     * Changes light status for the speicifed position
     * Sets the latest Red light position in case if it was red light
     * Invokes OnLightChanged() event
     * @param Position - position of the light
     * @param Light - new light status of this position
     */
    UFUNCTION(BlueprintCallable)
    void ChangeLight(ETrafficLightPosition Position, ETrafficLight Light);

    /**
     * Event that gets invoked when the light changes
     * Change material/vfx/sfv whaterever in BP
     * @paramOut Position - position of the changed light
     * @paramOut Light - new light status
     */
	UFUNCTION(BlueprintImplementableEvent, Category="Light")
	void OnLightChanged(const ETrafficLightPosition Position, const ETrafficLight Light);

    // ------------------------------------------------------------------
    // Lights status
    // ------------------------------------------------------------------

    /**
     * Current light status for the left light
     * Updates in ChangeLight()
     */
    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLight LeftLightStatus;

    /**
     * Current light status for the right light
     * Updates in ChangeLight()
     */
    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLight RightLightStatus;

    /**
     * Current light status for the center light
     * Updates in ChangeLight()
     */
    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLight CenterLightStatus;

    /**
     * Latest red light position
     * Updates in SetLatestRedLightPosition()
     */
    UPROPERTY(BlueprintReadWrite, Category="Light")
    ETrafficLightPosition LatestRedLightPosition;

    /**
     * Sets the latest red light position
     * Gets called in ChangeLight()
     * @param TrafficLightStatus - to check if it's red
     * @param TrafficLightPosition - to set the LatestRedLightPosition if the status is red
     */
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

    /** NO ATTACK */
    virtual void ExecuteAttack() override;

    /**
     * When the player enters the yellow light, pushes the player away
     * Determines the force of this push
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attack")
    float PlayerDragForce;

    /**
     * When the player enters the red light, emps the player
     * Determines the force of the emp
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Attack")
    float RedEmpForce;

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


    /**
     * Left arrow component
     * Determines the direction of the interaction with the player when he/she overlaps with the LeftLightMesh
     * Uses in LightBeginOverlap()
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Weapon")
    class UArrowComponent* LeftArrow;

    
    /**
     * Right arrow component
     * Determines the direction of the interaction with the player when he/she overlaps with the RightLightMesh
     * Uses in LightBeginOverlap()
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Weapon")
    class UArrowComponent* RightArrow;

    /**
     * Right arrow component
     * Determines the direction of the interaction with the player when he/she overlaps with the CenterLightMesh
     * Uses in LightBeginOverlap()
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Weapon")
    class UArrowComponent* CenterArrow;


protected:

    // ------------------------------------------------------------------
    // Missile
    // ------------------------------------------------------------------

    /**
     * When the missile enters the yellow/red light, throws it away
     * Determins the force of this throw
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Missile")
    float MissileThrowForce;

	/**
     * When the missile gets thrown away, invokes this event
     * Gets called in LightBeginOverlapEvent()
     * Do some fancy shit in bp
     * @paramOut Position - The light position
     * @paramOut Light - the current status of this light
     * @paramOut ImpactPosition - the position when the missile has entered the light
     * @paramOut bDefected - whenever the missile got defected or not (Yellow light - false, Red light - true)
     */
    UFUNCTION(BlueprintImplementableEvent, Category="Missile")
	void OnMissileGetThrownAway(const ETrafficLightPosition Position, const ETrafficLight Light, const FVector ImpactPosition, const bool bDefected);

    // ------------------------------------------------------------------
    // Lights overlaps
    // ------------------------------------------------------------------

    /**
     * Method that handles overlap for each light mesh
     * Handles missiles and the player
     * @param Actor - overlaped actor
     * @param TrafficLightStatus - status of the this light
     * @param TrafficLightPosition- position of the light
     */
    UFUNCTION(BlueprintCallable)
    void LightBeginOverlap(class AActor* Actor, const ETrafficLight TrafficLightStatus, const ETrafficLightPosition TrafficLightPosition);

    /**
     * Method that handles end overlap for each light mesh
     * Handles missiles and the player
     * @param Actor - overlaped actor
     * @param TrafficLightStatus - status of the this light
     * @param TrafficLightPosition- position of the light
     */
    UFUNCTION(BlueprintCallable)
    void LightEndOverlap(class AActor* Actor, const ETrafficLight TrafficLightStatus, const ETrafficLightPosition TrafficLightPosition);

    // ------------------------------------------------------------------
    // Lights Meshes
    // ------------------------------------------------------------------

    /**
     * Left light mesh
     * Use LightBeginOverlap Overlap handler
     */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UStaticMeshComponent* LeftLightMesh;

    /**
     * Right light mesh
     * Use LightBeginOverlap Overlap handler
     */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UStaticMeshComponent* RightLightMesh;

    /**
     * Center light mesh
     * Use LightBeginOverlap Overlap handler
     */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
    class UStaticMeshComponent* CenterLightMesh;

    /**
     * Hurtbox of the right position
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Weapon")
    class USphereComponent* HurtboxRight;

    /**
     * Hurtbox of the left position
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Weapon")
    class USphereComponent* HurtboxLeft;
    // ------------------------------------------------------------------
    // General
    // ------------------------------------------------------------------
	virtual void BeginPlay() override;

    /** Before attack event */
    virtual void BeforeAttackEvent_Implementation() override;
};

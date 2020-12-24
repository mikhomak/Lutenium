// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "FanMW.generated.h"

/**
 * Big ass fan weapon
 * Settels on the middle hight level of the monster
 * Normal attack - activate force pushing away in forward direction
 * Upgraded attack - two waves force (pushing and pulling)
 */
UCLASS()
class LUTENIUM_API AFanMW : public AMonsterWeapon
{
    GENERATED_BODY()
public:

    AFanMW();

    virtual void ExecuteAttack() override;

    // ------------------------------------------------------------------
	// Wind collisions
	// ------------------------------------------------------------------

    /**
     * Forward mesh to create a wind collision
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category= "Wind")
    class UStaticMeshComponent* ForwardWindCollision;

    /**
     * Backward mesh to create a wind collision
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category= "Wind")
    class UStaticMeshComponent* BackwardWindCollision;

    /**
     * Function called on BeginOverlap event
     * When player/missle enter the mesh collision, pushing it away or pulling it in depending on the bPush
     * @param OtherActor - actor that entered the collision
     * @param bPush - determins if the wind should push away or pull towards the fan
     */
    UFUNCTION(BlueprintCallable, Category= "Wind")
    void WindTriggerOverlap(class AActor* OtherActor, bool bPush);

    /**
     * Function called on ExitOverlap event
     * When player exit the mesh collision, deactivating movement effects
     * @param OtherActor - actor that entered the collision
     */
    UFUNCTION(BlueprintCallable, Category= "Wind")
    void WindTriggerOverlapEnd(class AActor* OtherActor);

    /**
     * Should forward wind be active?
     */
    UPROPERTY(BlueprintReadWrite, Category= "Wind")
    bool bForwardWindActive;

    /**
     * Should backward wind be active?
     */
    UPROPERTY(BlueprintReadWrite, Category= "Wind")
    bool bBackwardWindActive;

    /**
     * How long should wind be active for
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category= "Wind")
    float WindActiveTime;

    /**
     * Deactives wind after WindActiveTime has passed
     * Deactives both forward and backward wind.
     */
    FORCEINLINE UFUNCTION(BlueprintCallable, Category= "Wind")
    void DeactivateWind()
    {
        /* Invoking events for visuals n shit */
        if(bForwardWindActive)
        {
            OnDeactivateForwardWind(true);
        }
        if(bBackWindActive)
        {
            OnDeactivateForwardWind(true);
        }

        bForwardWindActive = false;
        bBackWindActive = false;
    }

    /**
     * Event on wind get actived
     * Do the particle shit in bp
     * @param bForward - true if that's forward wind, false if its backward
     */
    UFUNCTION(BlueprintImplementableEvent, Category="Wind")
    void OnActivateForwardWind(bool& bForward);

    /**
     * Event on wind get deactivated
     * Do the particle shit in bp
     * @param bForward - true if that's forward wind, false if its backward
     */
    UFUNCTION(BlueprintImplementableEvent, Category="Wind")
    void OnDeactivateForwardWind(bool& bForward);

    /**
     * Push force amount applied to the mesh on the player that overlaps wind
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category= "Wind")
    float PushForce;

    /**
     * Pull force amount applied to the mesh on the player that overlaps wind
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category= "Wind")
    float PullForce;
};

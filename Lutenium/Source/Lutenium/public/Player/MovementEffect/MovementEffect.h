#pragma once

#include "CoreMinimal.h"
#include "MovementEffect.generated.h"

/**
 * Abstract class for movement effect applying to the player
 * Could be activated(with method Activate()) with different parameters
 * When active(Active = true), each tick applies something(u wot) to the player movement(in method ApplyEffect())
 * To deactivate it call the method Deactivate()
 * Get the reference from the PlaneMovementComponent(this component has an array of all effects, but each effect has its own reference variable... just take a look at PlaneMovementComponent)
 * Each effect has safe timer(that means, after a certain time period it obligatory gets deactivated), that's kinda a safe feature
 */
UCLASS(BlueprintType, Blueprintable)
class LUTENIUM_API UMovementEffect : public UObject
{
    GENERATED_BODY()

public:

    UMovementEffect();

    /** Determines if this effect should be applied to the plane each tick or not */
    UPROPERTY(BlueprintReadWrite, Category = "Effect")
    bool Active;

    /** Variable to safety check to deactivate effect */
    UPROPERTY(BlueprintReadWrite, Category = "Effect")
    bool CanBeSafeDeactivated;

    /** Time it needs to pass to safety deactivate the effect */
    UPROPERTY(BlueprintReadWrite, Category = "Effect")
    float SafeDeactivateTime;

    /** Safe deactivation method. Only used after IsDeactivateTime has expired */
    FORCEINLINE UFUNCTION(BlueprintCallable, Category = "Effect")
    void SafeDeactivate()
    {
        if(Active && CanBeSafeDeactivated)
        {
            Active = false;
        }
    }

    /**
     * Starting timer for safe deactivation
     * @warning DO NOT FORGET TO ADD THIS METHOD IN ApplyEffect()!!!!!!
     */
    UFUNCTION(BlueprintCallable, Category = "Effect")
    void StartSafeDeactivation();

    /**
     * Virtual method to override in the child effects
     * Acivates the effect with different option for each effects
     */
    UFUNCTION(BlueprintCallable, Category = "Effect")
    virtual void ApplyEffect();

    /** Deactivating the effect */
    UFUNCTION(BlueprintCallable, Category = "Effect")
    void Deactivate();

    /** Called once while creating the effect to set needed references(kinda like constructor but with arguments so yeah) */
    FORCEINLINE UFUNCTION(BlueprintCallable, Category = "Effect")
    void InitEffect(class UPrimitiveComponent* PrimitiveComp, class UPlaneMovementComponent* PlaneMovementComp)
    {
        PrimitiveComponent = PrimitiveComp;
        PlaneMovementComponent = PlaneMovementComp;
        SafeDeactivateTime = 15.f;
    }

    /**
     * Virtual function to add specific logic for MovementEffect children if needed
     * Invokes in Deactivate()
     */
    UFUNCTION(BlueprintCallable, Category = "Effect")
    virtual void AdditionalDeactivationEffect();

    /**
     * Virtual function to add specific logic for MovementEffect children if needed
     * Invokes in Activate()
     */
    UFUNCTION(BlueprintCallable, Category = "Effect")
    virtual void AdditionalActivateEffect();

    /**
     * Debug property
     * Checks whenever we want to execute the code of the effect
     */
    UPROPERTY(BlueprintReadWrite, Category = "Effect")
    bool bDeactive_DEBUG;

protected:

    /** Reference to the player's box. Apply force n shit to this */
    class UPrimitiveComponent* PrimitiveComponent;

    /** Reference to the player's movement component */
    class UPlaneMovementComponent* PlaneMovementComponent;


};




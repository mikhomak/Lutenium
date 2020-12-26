#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerPawn.h"
#include "MovementEffect.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LUTENIUM_API UMovementEffect : public UObject
{
    GENERATED_BODY()

public:

    /** Determines if this effect should be applied to the plane each tick or not */
    UPROPERTY(BlueprintReadWrite, Category = "Effect")
    bool Active;

    /**
     * Virtual method to override in the child effects
     * Acivates the effect with different option for each effects
     */
    UFUNCTION(BlueprintCallable, Category = "Effect")
    virtual void ApplyEffect();

    /** Deactivating the effect */
    FORCEINLINE UFUNCTION(BlueprintCallable, Category = "Effect")
    void Deactivate() { Active = false; }

    /** Called once while creating the effect(int PlayerPawn) to set needed references */
    FORCEINLINE UFUNCTION(BlueprintCallable, Category = "Effect")
    void InitEffect(APlayerPawn* Pawn)
    {
        PlayerPawn = Pawn;
        PlayerBox = PlayerPawn->GetPlaneBox();
        PlaneMovementComp = PlayerPawn->GetPlaneMovement();
    }

protected:
    class APlayerPawn* PlayerPawn;

    class UBoxComponent* PlayerBox;

    class UPlaneMovementComponent* PlaneMovementComp;
};


inline void UMovementEffect::ApplyEffect(){}

#pragma once

#include "CoreMinimal.h"
#include "../public/Player/PlayerPawn.h"
#include "MovementEffect.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LUTENIUM_API UMovementEffect : public UObject
{
    GENERATED_BODY()

public:
    bool Active;

    UFUNCTION(BlueprintCallable, Category="Effect")
    virtual void ApplyEffect();

    UFUNCTION(BlueprintCallable, Category="Effect")
    virtual void Deactivate() { Active = false; }

    UFUNCTION(BlueprintCallable, Category="Effect")
    void InitEffect(APlayerPawn* Pawn);

protected:
    class APlayerPawn* PlayerPawn;

    class USkeletalMeshComponent* PlayerMesh;

    class UPlaneMovementComponent* PlaneMovementComp;
};


inline void UMovementEffect::ApplyEffect(){}

inline void UMovementEffect::InitEffect(APlayerPawn* Pawn)
{
    PlayerPawn = Pawn;
    PlayerMesh = PlayerPawn->GetPlaneMesh();
    PlaneMovementComp = PlayerPawn->GetPlaneComponent();
}
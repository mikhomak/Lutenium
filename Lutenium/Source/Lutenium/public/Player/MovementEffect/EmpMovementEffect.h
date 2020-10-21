#pragma once
#include "CoreMinimal.h"
#include "MovementEffect.h"
#include "EmpMovementEffect.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LUTENIUM_API UEmpMovementEffect : public UMovementEffect
{

    GENERATED_BODY()
public:
    UEmpMovementEffect(){};

    float EmpTime;

    FVector RotationDirection;

    float RotationForce;

    void ApplyEffect() override;

    UFUNCTION(BlueprintCallable, Category="Effect")
    void Activate(const float NewEmpTime, FVector NewRotationDirection, const float NewRotationForce);

    FORCEINLINE void Deactivate() override { Active = false; }
};

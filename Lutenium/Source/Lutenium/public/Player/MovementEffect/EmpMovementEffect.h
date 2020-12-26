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


    void ApplyEffect() override;

    UFUNCTION(BlueprintCallable, Category="Effect")
    void Activate(const float NewEmpTime, FVector NewRotationDirection, const float NewRotationForce);

protected:

    float EmpTime;

    FVector RotationDirection;

    float RotationForce;
};

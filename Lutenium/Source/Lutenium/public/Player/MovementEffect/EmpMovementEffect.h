#pragma once
#include "CoreMinimal.h"
#include "MovementEffect.h"
#include "EmpMovementEffect.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEmpActivateSignature);


UCLASS(BlueprintType, Blueprintable)
class LUTENIUM_API UEmpMovementEffect : public UMovementEffect
{

    GENERATED_BODY()
public:
    UEmpMovementEffect();


    void ApplyEffect() override;

    UFUNCTION(BlueprintCallable, Category="Effect")
    void Activate(FVector NewRotationDirection, const float NewRotationForce);

    UFUNCTION(BlueprintCallable, Category="Effect")
    void ActivateWithAddedGravity(FVector NewRotationDirection, const float NewRotationForce, const bool bAddGravity);

    UFUNCTION(BlueprintCallable, Category="Effect")
    void ActivateWithAddedGravityAndGravityValue(FVector NewRotationDirection, const float NewRotationForce, const bool bAddGravity, const float AddedGravity);

    UPROPERTY(BlueprintReadWrite, Category = "Effect")
    FVector RotationDirection;

    UPROPERTY(BlueprintReadWrite, Category = "Effect")
    float RotationForce;

    UPROPERTY(BlueprintReadWrite, Category = "Effect")
    float AdditionalGravity;

    UPROPERTY(BlueprintReadWrite, Category = "Effect")
    bool bAddAdditionalGravity;

    virtual void AdditionalDeactivationEffect() override;


    UPROPERTY(BlueprintAssignable, Category="Effect")
    FOnEmpActivateSignature OnEmpActivate;

    UPROPERTY(BlueprintAssignable, Category="Effect")
    FOnEmpActivateSignature OnEmpDeactivate;
};

#pragma once
#include "CoreMinimal.h"
#include "Player/MovementEffect/EmpMovementEffect.h"
#include "PlayerEmpMovementEffect.generated.h"


/**
 * EMP movement effect but invoking events for the player
 * Installation:
 * 		1. Change EmpMovementEffectClass in the construcor of UPlaneMovementComponent
 *		2. Set PlayerPawn to EmpMovementEffect
 */
UCLASS(BlueprintType, Blueprintable)
class LUTENIUM_API UPlayerEmpMovementEffect : public UEmpMovementEffect
{

    GENERATED_BODY()
public:
	/** Constructor */
    UPlayerEmpMovementEffect();

    /** Reference for the player. Needed to invokes effents */
    UPROPERTY(BlueprintReadWrite, Category = "Effect")
    class APlayerPawn* PlayerPawn;

    /** Invoking OnEmpActivateEvent() of APlayerPawn */
    virtual void AdditionalActivateEffect() override;

    /** Invoking OnEmpDeactivateEvent() of APlayerPawn */
    virtual void AdditionalDeactivationEffect() override;

};
#include "Player/MovementEffect/MovementEffect.h"
#include "TimerManager.h"


UMovementEffect::UMovementEffect()
{
    CanBeSafeDeactivated = true;
    SafeDeactivateTime = 15.f;
}

void UMovementEffect::StartSafeDeactivation()
{
    if(CanBeSafeDeactivated)
    {
        CanBeSafeDeactivated = false;
        FTimerHandle DeactivateTimer;
        PlayerPawn->GetWorldTimerManager().SetTimer(DeactivateTimer, this, &UMovementEffect::StartSafeDeactivation, SafeDeactivateTime, false);
    }
}

void UMovementEffect::ApplyEffect()
{

}

void UMovementEffect::AdditionalDeactivationEffect(){}
#include "Player/MovementEffect/MovementEffect.h"
#include "TimerManager.h"



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
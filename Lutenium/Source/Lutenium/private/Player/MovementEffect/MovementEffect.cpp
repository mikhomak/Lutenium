#include "Player/MovementEffect/MovementEffect.h"
#include "Player/PlaneMovementComponent.h"
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
        PlaneMovementComponent->GetWorld()->GetTimerManager().SetTimer(DeactivateTimer, this, &UMovementEffect::StartSafeDeactivation, SafeDeactivateTime, false);
    }
}

void UMovementEffect::Deactivate()
{
	Active = false;
	CanBeSafeDeactivated = true;
	AdditionalDeactivationEffect();
}

void UMovementEffect::ApplyEffect()
{

}

void UMovementEffect::AdditionalDeactivationEffect(){}

void UMovementEffect::AdditionalActivateEffect(){}
#include "Player/MovementEffect/PlayerEmpMovementEffect.h"
#include "Player/PlayerPawn.h"


UPlayerEmpMovementEffect::UPlayerEmpMovementEffect()
{

}

void UPlayerEmpMovementEffect::AdditionalActivateEffect()
{
	Super::AdditionalActivateEffect();


	// safety check
	if(PlayerPawn == nullptr)
	{
		return;
	}


	PlayerPawn->EmpActivateEffect();
}

void UPlayerEmpMovementEffect::AdditionalDeactivationEffect()
{
	Super::AdditionalDeactivationEffect();


	// safety check
	if(PlayerPawn == nullptr)
	{
		return;
	}


	PlayerPawn->EmpDeactivateEffect();
}
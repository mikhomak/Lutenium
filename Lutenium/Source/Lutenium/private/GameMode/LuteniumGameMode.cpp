#include "GameMode/LuteniumGameMode.h"
#include "Player/PlayerPawn.h"

ALuteniumGameMode::ALuteniumGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = APlayerPawn::StaticClass();
}

void ALuteniumGameMode::BeginPlay()
{
   	Super::BeginPlay();

}
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "LuteniumGameMode.h"
#include "PlayerPawn.h"

ALuteniumGameMode::ALuteniumGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = APlayerPawn::StaticClass();
}

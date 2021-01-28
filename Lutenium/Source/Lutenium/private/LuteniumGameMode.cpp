// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "../public/LuteniumGameMode.h"
#include "../public/Player/PlayerPawn.h"

ALuteniumGameMode::ALuteniumGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = APlayerPawn::StaticClass();
}

void ALuteniumGameMode::BeginPlay()
{
   	Super::BeginPlay();
	GetWorld()->bDebugDrawAllTraceTags = bDrawTraceDebug;
}
// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LuteniumGameMode.generated.h"

UCLASS(MinimalAPI)
class ALuteniumGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALuteniumGameMode();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Debug")
	bool bDrawCustomChangelTrace_DEBUG;

	virtual void BeginPlay() override;

};




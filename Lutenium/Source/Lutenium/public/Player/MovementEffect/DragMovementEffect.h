﻿#pragma once
#include "CoreMinimal.h"
#include "MovementEffect.h"
#include "DragMovementEffect.generated.h"

UCLASS(BlueprintType, Blueprintable)
class LUTENIUM_API UDragMovementEffect : public UMovementEffect
{

    GENERATED_BODY()
public:
    UDragMovementEffect(){};

    float DragForce;

    FVector DragDirection;

    void ApplyEffect() override;

    void Activate(const float NewDragForce, FVector& NewDragDirection);

    FORCEINLINE void Deactivate() override { Active = false; }
};
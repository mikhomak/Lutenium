#pragma once
#include "MovementEffectBuilder.h"

struct FDragMovementEffectBuilder : FMovementEffectBuilder
{
    FDragMovementEffectBuilder(float AppliedDragForce, const FVector& AppliedDragDirection);
    float DragForce;
    FVector DragDirection;
};

inline FDragMovementEffectBuilder::FDragMovementEffectBuilder(const float AppliedDragForce,
                                                              const FVector& AppliedDragDirection)
{
    DragForce = AppliedDragForce;
    DragDirection = AppliedDragDirection;
}

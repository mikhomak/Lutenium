#pragma once
#include "MovementEffectBuilder.h"

class FDragMovementEffectBuilder : public FMovementEffectBuilder
{
public:
    
    FDragMovementEffectBuilder(float AppliedDragForce, const FVector& AppliedDragDirection);

    float DragForce;

    FVector DragDirection;

    ~FDragMovementEffectBuilder() override = default;
};

inline FDragMovementEffectBuilder::FDragMovementEffectBuilder(const float AppliedDragForce,
                                                              const FVector& AppliedDragDirection)
{
    DragForce = AppliedDragForce;
    DragDirection = AppliedDragDirection;
}

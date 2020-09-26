#pragma once

#include "MovementEffect.h"


class FDragMovementEffect : FMovementEffect
{
public:

    FDragMovementEffect(class APlayerPawn* Pawn, class USkeletalMeshComponent* Mesh,
                        class UPlaneMovementComponent* PlaneMovementComponent);

    virtual void ApplyEffect() override;

    virtual void Activate(class FMovementEffectBuilder& Builder) override;

private:
    float DragForce;

    FVector DragDirection;
};

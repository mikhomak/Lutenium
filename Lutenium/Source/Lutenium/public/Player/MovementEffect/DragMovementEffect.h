#pragma once

#include "MovementEffect.h"


class FDragMovementEffect : public FMovementEffect
{
public:

    FDragMovementEffect();
    
    FDragMovementEffect(class APlayerPawn* Pawn, class USkeletalMeshComponent* Mesh,
                        class UPlaneMovementComponent* PlaneMovementComponent);

    void ApplyEffect() override;

    void Activate(class FMovementEffectBuilder* Builder) override;

private:
    float DragForce;

    FVector DragDirection;
};

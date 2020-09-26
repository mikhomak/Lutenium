#include "../public/Player/MovementEffect/DragMovementEffect.h"
#include "../public/Player/PlaneMovementComponent.h"
#include "../public/Player/PlayerPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/MovementEffect/DragMovementEffectBuilder.h"

FDragMovementEffect::FDragMovementEffect(class APlayerPawn* Pawn, class USkeletalMeshComponent* Mesh,
                                         class UPlaneMovementComponent* PlaneMovementComponent): DragForce(0)
{
    PlayerPawn = Pawn;
    PlayerMesh = Mesh;
    PlaneMovementComp = PlaneMovementComponent;
}

void FDragMovementEffect::ApplyEffect()
{
    if (Active)
    {
        PlayerMesh->AddForce(DragDirection * DragForce, FName(), true);
    }
}

void FDragMovementEffect::Activate(FMovementEffectBuilder& Builder)
{
    if (FDragMovementEffectBuilder* DragBuilder = dynamic_cast<FDragMovementEffectBuilder*>(Builder))
    {
        DragForce = DragBuilder->DragForce;
        DragDirection = DragBuilder->DragDirection;
        if (!DragDirection.IsNormalized())
        {
            DragDirection.Normalize();
        }
        Active = true;
    }
}

#include "../public/Player/MovementEffect/DragMovementEffect.h"
#include "../public/Player/MovementEffect/MovementEffectBuilder.h"
#include "../public/Player/PlaneMovementComponent.h"
#include "../public/Player/PlayerPawn.h"
#include "../public/Player/MovementEffect/DragMovementEffectBuilder.h"
#include "Components/SkeletalMeshComponent.h"

FDragMovementEffect::FDragMovementEffect(): DragForce(0)
{
}

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

void FDragMovementEffect::Activate(FMovementEffectBuilder* Builder)
{
    FDragMovementEffectBuilder* DragBuilder = static_cast<FDragMovementEffectBuilder*>(Builder);
    if (DragBuilder)
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

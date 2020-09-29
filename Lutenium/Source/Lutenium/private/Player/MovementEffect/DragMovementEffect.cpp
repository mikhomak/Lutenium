#include "../public/Player/MovementEffect/DragMovementEffect.h"
#include "../public/Player/PlaneMovementComponent.h"
#include "../public/Player/PlayerPawn.h"
#include "Components/SkeletalMeshComponent.h"


void UDragMovementEffect::ApplyEffect()
{
    if (Active)
    {
        PlayerMesh->AddForce(DragDirection * DragForce, FName(), true);
    }
}

void UDragMovementEffect::Activate(const float NewDragForce, FVector& NewDragDirection)
{
    DragForce = NewDragForce;
    DragDirection = NewDragDirection;
    if (!DragDirection.IsNormalized())
    {
        DragDirection.Normalize();
    }
    Active = true;
}

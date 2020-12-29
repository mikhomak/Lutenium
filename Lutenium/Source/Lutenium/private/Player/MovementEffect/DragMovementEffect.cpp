#include "Player/MovementEffect/DragMovementEffect.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/PlayerPawn.h"
#include "Components/BoxComponent.h"



void UDragMovementEffect::ApplyEffect()
{
    if (Active)
    {
        PlayerBox->AddForce(DragDirection * DragForce, FName(), true);
    }
}

void UDragMovementEffect::Activate(const float NewDragForce, FVector NewDragDirection)
{
    DragForce = NewDragForce;
    DragDirection = NewDragDirection;
    if (!DragDirection.IsNormalized())
    {
        DragDirection.Normalize();
    }
    Active = true;
    StartSafeDeactivation();
}

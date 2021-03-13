#include "Player/MovementEffect/DragMovementEffect.h"
#include "Player/PlayerPawn.h"
#include "Components/PrimitiveComponent.h"



void UDragMovementEffect::ApplyEffect()
{
    if (Active)
    {
        PrimitiveComponent->AddForce(DragDirection * DragForce, FName(), true);
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
    AdditionalActivateEffect();
}

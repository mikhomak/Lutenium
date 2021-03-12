#include "Player/MovementEffect/EmpMovementEffect.h"
#include "Player/PlaneMovementComponent.h"
#include "Components/PrimitiveComponent.h"


UEmpMovementEffect::UEmpMovementEffect()
{
    AdditionalGravity = -900.f;
}

void UEmpMovementEffect::ApplyEffect()
{
    if (Active && !bDeactive_DEBUG)
    {
        const FVector DirectionToTilt = FMath::Lerp(FVector::ZeroVector, RotationDirection * RotationForce, 0.1f);
        PrimitiveComponent->AddTorqueInRadians(DirectionToTilt, FName(), true);
        // Adding additional gravity
        if(bAddAdditionalGravity)
        {
            PrimitiveComponent->AddForce(FVector(0, 0, AdditionalGravity), FName(), true);
        }
    }
}

void UEmpMovementEffect::Activate(FVector NewRotationDirection, const float NewRotationForce)
{
    if(PlaneMovementComponent && !Active)
    {
        if(!PlaneMovementComponent->bStalling)
        {
            RotationForce = NewRotationForce;
            RotationDirection = NewRotationDirection;
            if (!RotationDirection.IsNormalized())
            {
                RotationDirection.Normalize();
            }
            Active = true;
            PlaneMovementComponent->bStalling = true;
            PlaneMovementComponent->ResetCurrentAcceleration();
        }
    }
    StartSafeDeactivation();
}


void UEmpMovementEffect::ActivateWithAddedGravity(FVector NewRotationDirection, const float NewRotationForce, const bool bAddGravity)
{
    Activate(NewRotationDirection, NewRotationForce);
    bAddAdditionalGravity = bAddGravity;
}

void UEmpMovementEffect::ActivateWithAddedGravityAndGravityValue(FVector NewRotationDirection, const float NewRotationForce, const bool bAddGravity, const float AddedGravity)
{
    ActivateWithAddedGravity(NewRotationDirection, NewRotationForce, bAddGravity);
    AdditionalGravity = AddedGravity;
}


void UEmpMovementEffect::AdditionalDeactivationEffect()
{
    bAddAdditionalGravity = false;
    AdditionalGravity = -900.f;
}

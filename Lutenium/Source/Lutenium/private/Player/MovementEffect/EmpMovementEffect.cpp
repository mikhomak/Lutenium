#include "Player/MovementEffect/EmpMovementEffect.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/PlayerPawn.h"
#include "Components/BoxComponent.h"


UEmpMovementEffect::UEmpMovementEffect()
{
    AdditionalGravity = -900.f;
}

void UEmpMovementEffect::ApplyEffect()
{
    if (Active)
    {
        const FVector ZeroVector;
        const FVector DirectionToTilt = FMath::Lerp(ZeroVector, RotationDirection * RotationForce, 0.1f);
        PlayerBox->AddTorqueInRadians(DirectionToTilt, FName(), true);
        // Adding additional gravity
        if(bAddAdditionalGravity)
        {
            PlayerBox->AddForce(FVector(0, 0, AdditionalGravity), FName(), true);
        }
    }
}

void UEmpMovementEffect::Activate(FVector NewRotationDirection, const float NewRotationForce)
{
    if(PlaneMovementComp && !Active)
    {
        if(!PlaneMovementComp->bStalling)
        {
            RotationForce = NewRotationForce;
            RotationDirection = NewRotationDirection;
            if (!RotationDirection.IsNormalized())
            {
                RotationDirection.Normalize();
            }
            Active = true;
            PlaneMovementComp->bStalling = true;
            PlaneMovementComp->ResetCurrentAcceleration();
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

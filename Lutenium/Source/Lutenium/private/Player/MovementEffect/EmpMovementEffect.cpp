#include "Player/MovementEffect/EmpMovementEffect.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/PlayerPawn.h"
#include "Components/BoxComponent.h"

void UEmpMovementEffect::ApplyEffect()
{
    if (Active)
    {
        const FVector ZeroVector;
        const FVector DirectionToTilt = FMath::Lerp(ZeroVector, RotationDirection * RotationForce, 0.1f);
        PlayerBox->AddTorqueInRadians(DirectionToTilt, FName(), true);
    }
}

void UEmpMovementEffect::Activate(const float NewEmpTime, FVector NewRotationDirection, const float NewRotationForce)
{
    if(PlaneMovementComp)
    {
        if(!PlaneMovementComp->bStalling)
        {
            EmpTime = NewEmpTime;
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
}

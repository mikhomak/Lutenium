#include "../../public/AssistUtils/AssistUtils.h"
#include "../../public/Player/PlaneMovementComponent.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"

bool FAssistUtils::bHasApplied = false;
float FAssistUtils::RaycastMissileTarget_Length = 500000.f;
float FAssistUtils::RaycastMissile_First_Radius = 750.f;
float FAssistUtils::RaycastMissile_Second_Radius = 1500.f;

void FAssistUtils::ApplyTakeOffAcceleration(UPlaneMovementComponent* PlaneMovementComponent, const FVector& Velocity,
                                            const float MaxSpeedUntilTakeOff, const float AddedAcceleration)
{
    if (!FAssistUtils::bHasApplied && Velocity.Size() < MaxSpeedUntilTakeOff)
    {
        PlaneMovementComponent->AddAcceleration(AddedAcceleration);
        bHasApplied = true;
    }
    else if (Velocity.Size() > MaxSpeedUntilTakeOff)
    {
        FAssistUtils::bHasApplied = false;
    }
}

FVector FAssistUtils::RaycastMissileTarget(const UWorld* World, const FVector& StartLocation,
                                           const FVector& ForwardVector)
{
    if (World)
    {
        FHitResult FirstHitResult;

        const FVector EndLocation = StartLocation + ForwardVector * FAssistUtils::RaycastMissileTarget_Length;
        const bool bFirstHit = World->SweepSingleByChannel(FirstHitResult,
                                                           StartLocation,
                                                           EndLocation,
                                                           FQuat::Identity,
                                                           ECollisionChannel::ECC_Pawn,
                                                           FCollisionShape::MakeSphere(RaycastMissile_First_Radius));
        if (bFirstHit)
        {
            return FirstHitResult.Location;
        }

        FHitResult SecondHitResult;
        const bool bSecondHit = World->SweepSingleByChannel(SecondHitResult,
                                                            StartLocation,
                                                            EndLocation,
                                                            FQuat::Identity,
                                                            ECollisionChannel::ECC_Pawn,
                                                            FCollisionShape::MakeSphere(RaycastMissile_Second_Radius));
        if (bSecondHit)
        {
            return SecondHitResult.Location;
        }

        return EndLocation;
    }
    return FVector();
}

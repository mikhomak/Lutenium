#include "../../public/AssistUtils/AssistUtils.h"
#include "../../public/PlaneMovementComponent.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"

bool FAssistUtils::bHasApplied = false;

void FAssistUtils::ApplyTakeOffAcceleration(UPlaneMovementComponent* PlaneMovementComponent, const FVector& Velocity,
                                            const float MinTakeOffVelocity, const float AddedAcceleration)
{
    if (!FAssistUtils::bHasApplied && Velocity.Size() < MinTakeOffVelocity)
    {
        PlaneMovementComponent->AddAcceleration(AddedAcceleration);
        bHasApplied = true;
    }
    else if (Velocity.Size() > MinTakeOffVelocity)
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

        const FVector EndLocation = StartLocation + ForwardVector * RaycastMissileTarget_Length;
        const bool bFirstHit = World->SweepSingleByChannel(FirstHitResult,
                                                           StartLocation,
                                                           EndLocation,
                                                           FQuat::Identity,
                                                           ECollisionChannel::ECC_GameTraceChannel1,
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
                                                            ECollisionChannel::ECC_GameTraceChannel1,
                                                            FCollisionShape::MakeSphere(RaycastMissile_Second_Radius));
        if (bSecondHit)
        {
            return SecondHitResult.Location;
        }

        return EndLocation;
    }
    return FVector();
}

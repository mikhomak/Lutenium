#include "../../public/AssistUtils/AssistUtils.h"
#include "../../public/Player/PlaneMovementComponent.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"

bool FAssistUtils::bHasApplied = false;
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

USceneComponent* FAssistUtils::RaycastMissileTarget(const AActor* Actor, const UWorld* World,
                                                                   const FVector& StartLocation,
                                                                   const FVector& ForwardVector,
                                                                   const float& TraceLength,
                                                                   FVector& HitLocation)
{
    const FVector EndLocation = StartLocation + ForwardVector * TraceLength;
    if (World)
    {
        // Ignoring the actor
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Actor);


        /*
         * Raycasting the first Sphere
         * The smallest one
         * Only searching for pawns 
         */
        FHitResult FirstHitResult;
        const bool bFirstHit = World->SweepSingleByChannel(FirstHitResult,
                                                           StartLocation,
                                                           EndLocation,
                                                           FQuat::Identity,
                                                           ECollisionChannel::ECC_Pawn,
                                                           FCollisionShape::MakeSphere(RaycastMissile_First_Radius),
                                                           Params);
        if (bFirstHit && FirstHitResult.GetComponent())
        {
            HitLocation = FirstHitResult.Location;
            return FirstHitResult.GetComponent();
        }

        /*
        * Raycasting the second Sphere
        * If the first raycast failed, then we are raycasting the bigger area
        * The biggest one
        * Only searching for pawns 
        */
        FHitResult SecondHitResult;
        const bool bSecondHit = World->SweepSingleByChannel(SecondHitResult,
                                                            StartLocation,
                                                            EndLocation,
                                                            FQuat::Identity,
                                                            ECollisionChannel::ECC_Pawn,
                                                            FCollisionShape::MakeSphere(RaycastMissile_Second_Radius),
                                                            Params);
        if (bSecondHit && SecondHitResult.GetComponent())
        {
            HitLocation = SecondHitResult.Location;
            return SecondHitResult.GetComponent();
        }

        /*
         * Raycasting only forward line
         * If the second one is failed (probably the enemy pawns are too far from the initial missile forward vector)
         * Returning nullptr in any case
         * Setting the location if we hit something or just the EndLocation(going on a straight line)
         * Channel is Visibility
         */


        FHitResult FinalHitResult;
        const bool bFinalHit = World->LineTraceSingleByChannel(FinalHitResult,
                                                               StartLocation,
                                                               EndLocation,
                                                               ECollisionChannel::ECC_Visibility,
                                                               Params);

        HitLocation = bFinalHit ? FinalHitResult.Location : EndLocation;
        return nullptr;
    }
    HitLocation = EndLocation;
    return nullptr;
}

#include "../../public/AssistUtils/AssistUtils.h"
#include "../../public/Player/PlaneMovementComponent.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"
#define ECC_MonsterWPHurtbox ECollisionChannel::ECC_GameTraceChannel1

USceneComponent* FAssistUtils::RaycastMissileTarget(const AActor* Actor, const UWorld* World,
                                                                   const FVector& StartLocation,
                                                                   const FVector& ForwardVector,
                                                                   const float& TraceLength,
                                                                   const float& FirstRaycastRadius,
                                                                   const float& SecondRaycastRadius,
                                                                   FVector& HitLocation)
{
    const FVector EndLocation = StartLocation + ForwardVector * TraceLength;
    if (World)
    {
        // Ignoring the actor
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Actor);
        FCollisionObjectQueryParams ObjectQuery(ECC_MonsterWPHurtbox);

        /*
         * Raycasting the first Sphere
         * The smallest one
         * Only searching for pawns
         */
        FHitResult FirstHitResult;
        const bool bFirstHit = World->SweepSingleByObjectType(FirstHitResult,
                                                           StartLocation,
                                                           EndLocation,
                                                           FQuat::Identity,
                                                           ObjectQuery,
                                                           FCollisionShape::MakeSphere(FirstRaycastRadius),
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
        const bool bSecondHit = World->SweepSingleByObjectType(SecondHitResult,
                                                            StartLocation,
                                                            EndLocation,
                                                            FQuat::Identity,
                                                            ObjectQuery,
                                                            FCollisionShape::MakeSphere(SecondRaycastRadius),
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

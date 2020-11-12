#include "../../public/AssistUtils/AssistUtils.h"
#include "../../public/Player/PlaneMovementComponent.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"
#define ECC_MonsterWPHurtbox ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Monster ECollisionChannel::ECC_GameTraceChannel2

USceneComponent* FAssistUtils::RaycastMissileTarget(const AActor* Actor, const UWorld* World,
                                                                   const FVector& StartLocation,
                                                                   const FVector& ForwardVector,
                                                                   const float& TraceLength,
                                                                   const float& FirstRaycastRadius,
                                                                   const float& SecondRaycastRadius,
                                                                   FVector& HitLocation,
                                                                   bool& bMonsterHurtboxOrMonster)
{
    const FVector EndLocation = StartLocation + ForwardVector * TraceLength;
    if (World)
    {
        // Ignoring the actor
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Actor);
        FCollisionObjectQueryParams MonsterWPHurtBoxObjectQuery(ECC_MonsterWPHurtbox);

        /*
         * Raycasting the first Sphere looking for a monster weapon hurtbox
         * If we foud one, check the visibility raycast. If it is not blocking, we can shoot the thing -> return the weapon
         * If it is blocking, then check if the block was by a monster
         */
        FHitResult FirstHitResult;
        const bool bFirstHit = World->SweepSingleByObjectType(
                                                           FirstHitResult,
                                                           StartLocation,
                                                           EndLocation,
                                                           FQuat::Identity,
                                                           MonsterWPHurtBoxObjectQuery,
                                                           FCollisionShape::MakeSphere(FirstRaycastRadius),
                                                           Params);
        if (bFirstHit && FirstHitResult.GetComponent())
        {
            FHitResult VisibilityHitResult;
            const bool bVisibilityHit =  World->LineTraceSingleByChannel(
                                                VisibilityHitResult,
                                                StartLocation,
                                                EndLocation,
                                                ECC_Visibility,
                                                Params);
            if(!bVisibilityHit)
            {
                bMonsterHurtboxOrMonster = true;
                HitLocation = FirstHitResult.Location;
                return FirstHitResult.GetComponent();
            }
        }

        /*
        * Raycasting the second Sphere
        * If the first raycast failed, then we are raycasting to see if the blocking hit was a monster
        * If it is, returnign the monster object(it could be a monster weapon or a monster itself)
        * If there was no blocking hit or it wasn't monster, then just return the zero vector because what do you expect
        */
        FCollisionObjectQueryParams MonsterObjectQuery(ECC_Monster);
        FHitResult SecondHitResult;
        const bool bSecondHit = World->SweepSingleByObjectType(SecondHitResult,
                                                            StartLocation,
                                                            EndLocation,
                                                            FQuat::Identity,
                                                            MonsterObjectQuery,
                                                            FCollisionShape::MakeSphere(SecondRaycastRadius),
                                                            Params);
        if (bSecondHit && SecondHitResult.GetComponent())
        {
            HitLocation = SecondHitResult.Location;
            return SecondHitResult.GetComponent();
        }
    }
    HitLocation = FVector::ZeroVector;
    return nullptr;
}

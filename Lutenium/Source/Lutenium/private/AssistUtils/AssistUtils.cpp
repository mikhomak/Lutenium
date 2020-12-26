#include "AssistUtils/AssistUtils.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/MissileTargetHit.h"
#include "Player/PlayerPawn.h"
#include "Math/Vector.h"
#include "Kismet/KismetSystemLibrary.h"
#define ECC_MonsterWPHurtbox ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Monster ECollisionChannel::ECC_GameTraceChannel2
#define ECC_Player ECollisionChannel::ECC_GameTraceChannel4

USceneComponent* FAssistUtils::RaycastMissileTarget(const AActor* Actor, const UWorld* World,
                                                                   const FVector& StartLocation,
                                                                   const FVector& ForwardVector,
                                                                   const float TraceLength,
                                                                   const float FirstRaycastRadius,
                                                                   const float SecondRaycastRadius,
                                                                   FVector& HitLocation,
                                                                   EMissileTargetHit& MissileTargetHitType)
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
                MissileTargetHitType = EMissileTargetHit::MonsterWPHurtbox;
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
            MissileTargetHitType = EMissileTargetHit::Monster;
            HitLocation = SecondHitResult.Location;
            return nullptr;
        }
    }
    MissileTargetHitType = EMissileTargetHit::NoHit;
    HitLocation = FVector::ZeroVector;
    return nullptr;
}

/** Racyast the player (use for mosnter) */
/** Raycast ONLY the player level of collision */
/** Returns Player and HitResultOut */
APlayerPawn* FAssistUtils::RaycastForPlayer(const AActor* OwnerActor,
                                            const UWorld* World,
                                            const FVector& StartLocation,
                                            const FVector& EndLocation,
                                            const float RaycastRadius,
                                            FHitResult& HitResultOut)
{
    if(World)
    {
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(OwnerActor);

        FCollisionObjectQueryParams PlayerObjectQuery(ECC_Player);

        const bool bHit = World->SweepSingleByObjectType(HitResultOut,
                                                         StartLocation,
                                                         EndLocation,
                                                         FQuat::Identity,
                                                         PlayerObjectQuery,
                                                         FCollisionShape::MakeSphere(RaycastRadius),
                                                         Params);
        if(bHit && HitResultOut.Actor != nullptr)
        {
            APlayerPawn* Player =  Cast<APlayerPawn>(HitResultOut.Actor);
            if(Player)
            {
                return Player;
            }
        }
    }

    return nullptr;
}


/**
 * Same funcinality as the normal RaycastForPlayer but has the second end location in case if the first one hasn't found the player
 */
APlayerPawn* FAssistUtils::RaycastForPlayer(const AActor* OwnerActor,
                                            const UWorld* World,
                                            const FVector& StartLocation,
                                            const FVector& EndLocation,
                                            const FVector& SecondEndLocation,
                                            const float RaycastRadius,
                                            FHitResult& HitResultOut)
{
    APlayerPawn* Player = RaycastForPlayer(OwnerActor, World, StartLocation, EndLocation, RaycastRadius, HitResultOut);
    if(Player)
    {
        return Player;
    }
    return RaycastForPlayer(OwnerActor, World, StartLocation, SecondEndLocation, RaycastRadius, HitResultOut);
}
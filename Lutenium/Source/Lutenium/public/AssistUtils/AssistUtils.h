#pragma once
#include "Player/MissileTargetHit.h"

class UPlaneMovementComponent;
struct FVector;

class FAssistUtils
{
public:

    static class USceneComponent* RaycastMissileTarget(const TSet<AActor*>& ActorsToIgnore,
                                                       const UWorld* World,
                                                       const FVector& StartLocation,
                                                       const FVector& ForwardVector,
                                                       const float TraceLength,
                                                       const float RaycastRadius,
                                                       const float SecondRaycastRadius,
                                                       FVector& HitLocation,
                                                       EMissileTargetHit& MissileTargetHitType,
                                                       AActor*& HitActor);

    static class USceneComponent* RaycastUpgradedMissileTarget(const TSet<AActor*>& ActorsToIgnore,
                                                               const UWorld* World,
                                                               const FVector& StartLocation,
                                                               const FVector& ForwardVector,
                                                               const float TraceLength,
                                                               const float FirstRaycastRadius,
                                                               FVector& HitLocation,
                                                               AActor*& HitActor);

    static bool RaycastSameMonsterPosition(const TSet<AActor*>& ActorsToIgnore,
                                                 const UWorld* World,
                                                 const FVector& StartLocation,
                                                 FVector& SameMonsterLocation);

    static class AActor* RaycastForPlayer(const AActor* OwnerActor,
                                               const UWorld* World,
                                               const FVector& StartLocation,
                                               const FVector& EndLocation,
                                               const float RaycastRadius,
                                               FHitResult& HitResultOut);

    static class AActor* RaycastForPlayer(const AActor* OwnerActor,
                                               const UWorld* World,
                                               const FVector& StartLocation,
                                               const FVector& EndLocation,
                                               const FVector& SecondEndLocation,
                                               const float RaycastRadius,
                                               FHitResult& HitResultOut);
};

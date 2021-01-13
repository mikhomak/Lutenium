#pragma once
#include "Player/MissileTargetHit.h"

class UPlaneMovementComponent;
struct FVector;

class FAssistUtils
{
public:

    static class USceneComponent* RaycastMissileTarget(const AActor* Actor,
                                                       const UWorld* World,
                                                       const FVector& StartLocation,
                                                       const FVector& ForwardVector,
                                                       const float TraceLength,
                                                       const float FirstRaycastRadius,
                                                       const float SecondRaycastRadius,
                                                       FVector& HitLocation,
                                                       EMissileTargetHit& MissileTargetHitType);

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

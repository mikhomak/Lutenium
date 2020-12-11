﻿#pragma once
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

    static class APlayerPawn* RaycastForPlayer(const AActor* OwnerActor,
                                               const UWorld* World,
                                               const FVector& StartLocation,
                                               const FVector& ForwardVector,
                                               const float TraceLength,
                                               const float RaycastRadius,
                                               FHitResult& HitResult);
};

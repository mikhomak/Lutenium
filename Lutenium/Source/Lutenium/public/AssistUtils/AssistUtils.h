#pragma once

class UPlaneMovementComponent;
struct FVector;

class FAssistUtils
{
public:
    static void ApplyTakeOffAcceleration(UPlaneMovementComponent* PlaneMovementComponent, const FVector& Velocity,
                                         const float MinTakeOffVelocity, const float AddedAcceleration);

    static class USceneComponent* RaycastMissileTarget(const AActor* Actor,
                                                       const UWorld* World,
                                                       const FVector& StartLocation,
                                                       const FVector& ForwardVector,
                                                       const float& TraceLength,
                                                       const float& FirstRaycastRadius,
                                                       const float& SecondRaycastRadius,
                                                       FVector& HitLocation);
private:
    static bool bHasApplied;
};

#pragma once

class UPlaneMovementComponent;
struct FVector;

class FAssistUtils
{
public:

    static class USceneComponent* RaycastMissileTarget(const AActor* Actor,
                                                       const UWorld* World,
                                                       const FVector& StartLocation,
                                                       const FVector& ForwardVector,
                                                       const float& TraceLength,
                                                       const float& FirstRaycastRadius,
                                                       const float& SecondRaycastRadius,
                                                       FVector& HitLocation);
private:
};

#pragma once

class UPlaneMovementComponent;
struct FVector;

class FAssistUtils
{
public:
    static void ApplyTakeOffAcceleration(UPlaneMovementComponent* PlaneMovementComponent, const FVector& Velocity,
                                         const float MinTakeOffVelocity, const float AddedAcceleration);

    static FVector RaycastMissileTarget(const UWorld* World, const FVector& StartLocation,
                                        const FVector& ForwardVector);
private:
    static bool bHasApplied;

    static float RaycastMissileTarget_Length;

    static float RaycastMissile_First_Radius;

    static float RaycastMissile_Second_Radius;
};

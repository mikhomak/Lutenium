#pragma once

class UPlaneMovementComponent;
struct FVector;

class FAssistUtils
{
public:
    static void ApplyTakeOffAcceleration(UPlaneMovementComponent* PlaneMovementComponent,
                                         FVector Velocity, float MinTakeOffVelocity, float AddedAcceleration);
private:
    static bool bHasApplied;
};

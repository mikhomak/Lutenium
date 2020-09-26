#pragma once

class FMovementEffect
{
public:

    virtual ~FMovementEffect() = default;

    virtual void ApplyEffect();

    virtual void Activate(class FMovementEffectBuilder& Builder);
protected:
    bool Active = false;

    class APlayerPawn* PlayerPawn;

    class USkeletalMeshComponent* PlayerMesh;

    class UPlaneMovementComponent* PlaneMovementComp;
public:
    FORCEINLINE void Deactivate() { Active = false; }
};

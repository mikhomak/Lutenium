#include "Player/PlayerPlaneMovementComponent.h"
#include "Player/PlayerPawn.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"

UPlayerPlaneMovementComponent::UPlayerPlaneMovementComponent()
{
	DashImpactForce = 8000.f;
    MaxDashes = 2;
    DashesLeft = MaxDashes;
    DashCooldown = 3.f;
    bCanDash = true;
}

void UPlayerPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerPlaneMovementComponent::DashInput()
{
    if (DashesLeft <= 0)
    {
        return;
    }
    bCanDash = false;
    PlayerPawn->DashImpact();
    PhysicsComponent->AddForce(PhysicsComponent->GetForwardVector() * DashImpactForce, FName(), true);
    CurrentAcceleration = MaxThrustUpAcceleration;
    --DashesLeft;
    FTimerHandle DashCooldownTimer;
    GetWorld()->GetTimerManager().SetTimer(DashCooldownTimer, this, &UPlayerPlaneMovementComponent::ResetDashCooldown,
                                           DashCooldown,
                                           false);
}


void UPlayerPlaneMovementComponent::ResetDashCooldown()
{
    bCanDash = true;
    if (DashesLeft < MaxDashes)
    {
        DashesLeft++;
    }
}


void UPlayerPlaneMovementComponent::HasDotChangedEventCaller()
{
	PlayerPawn->DotHasChange();
}
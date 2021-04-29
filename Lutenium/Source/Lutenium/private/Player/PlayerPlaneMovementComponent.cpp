#include "Player/PlayerPlaneMovementComponent.h"
#include "Player/PlayerPawn.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "TimerManager.h"

UPlayerPlaneMovementComponent::UPlayerPlaneMovementComponent()
{
	DashImpactForce = 8000.f;
    MaxDashes = 2;
    DashesLeft = MaxDashes;
    DashCooldown = 3.f;
    bCanDash = true;

    TimeToBeAbleToDeactivateTravelMode = 0.05f;
}

void UPlayerPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UPlayerPlaneMovementComponent::BeginPlay()
{
    Super::BeginPlay();

}


void UPlayerPlaneMovementComponent::DashInput()
{
    if (DashesLeft <= 0)
    {
        return;
    }
    bCanDash = false;
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


void UPlayerPlaneMovementComponent::ThrustInput(const FInputActionValue& Value)
{
    Super::ThrustInput(Value.GetMagnitude());
    // Safety deactivation of the target mode
    // If the jet is in travelMode, and Thrust is negative(aka the player is pressing stop button) deactivates the travel mode
    // We don't care about bIsAbleToDeactivateTravelMode cuz if the player is pressing stop button, that probably means that she/he doesn't want to enter the travel mode
    if(bIsInTravelMode && Value.GetMagnitude() < 0)
    {
        DeactivateTravelMode();
    }
}

void UPlayerPlaneMovementComponent::ActivateTravelMode()
{
    // Upgrade boolean
    // If the player doesn't have an upgrade, do nothing
    if(!bCanTravelMode)
    {
        return;
    }

    OnTravelModeActivate.Broadcast();
    bIsInTravelMode = true;
    AirControl = AirControl * TravelModeAircontrolMultiplier;
    MaxAcceleration = MaxAcceleration *  TravelModeMaxAccelerationMultiplier;
    MaxThrustUpAcceleration = MaxThrustUpAcceleration *  TravelModeMaxAccelerationMultiplier;
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlayerPlaneMovementComponent::CanDeactivateTravelMode,
                                           TimeToBeAbleToDeactivateTravelMode, false);

}


void UPlayerPlaneMovementComponent::DeactivateTravelMode()
{
    // Upgrade boolean
    // If the player doesn't have an upgrade, do nothing
    if(!bCanTravelMode)
    {
        return;
    }

    // If the player is not in travel mode or is not able to deactivate it, do nothing
    if(!bIsInTravelMode || !bIsAbleToDeactivateTravelMode)
    {
        return;
    }

    // The player is in travel mode and is able to deactivate it
    OnTravelModeDeactivate.Broadcast();
    AirControl = AirControl / TravelModeAircontrolMultiplier;
    MaxAcceleration = MaxAcceleration / TravelModeMaxAccelerationMultiplier; 
    MaxThrustUpAcceleration = MaxThrustUpAcceleration / TravelModeMaxAccelerationMultiplier ;
    bIsInTravelMode = false;
    bIsAbleToDeactivateTravelMode = false;
}
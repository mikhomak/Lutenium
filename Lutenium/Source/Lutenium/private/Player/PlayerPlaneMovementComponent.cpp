#include "Player/PlayerPlaneMovementComponent.h"
#include "Player/PlayerPawn.h"
#include "Player/MovementEffect/PlayerEmpMovementEffect.h"
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

    TimeToBeAbleToDeactivateTravelMode = 1.f;

    EmpMovementEffectClass = UPlayerEmpMovementEffect::StaticClass();
}

void UPlayerPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


void UPlayerPlaneMovementComponent::BeginPlay()
{
    Super::BeginPlay();
    if(Cast<UPlayerEmpMovementEffect>(EmpMovementEffect) != nullptr)
    {
        ((UPlayerEmpMovementEffect*) EmpMovementEffect)->PlayerPawn = PlayerPawn;
    }
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


void UPlayerPlaneMovementComponent::ThrustInput(const FInputActionValue& Value)
{
    Super::ThrustInput(Value.GetMagnitude());
}

void UPlayerPlaneMovementComponent::ActivateTravelMode()
{
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
    if(!bCanTravelMode)
    {
        return;
    }

    if(!bIsInTravelMode || !bIsAbleToDeactivateTravelMode)
    {
        return;
    }

    OnTravelModeDeactivate.Broadcast();
    AirControl = AirControl / TravelModeAircontrolMultiplier;
    MaxAcceleration = MaxAcceleration / TravelModeMaxAccelerationMultiplier; 
    MaxThrustUpAcceleration = MaxThrustUpAcceleration / TravelModeMaxAccelerationMultiplier ;
    bIsInTravelMode = false;
    bIsAbleToDeactivateTravelMode = false;
}
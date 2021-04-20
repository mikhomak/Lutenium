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

void UPlayerPlaneMovementComponent::ReleasedThrustInput()
{
    if(TravelModeThurstActivations == 1 && bCanEnterTravelMode == false)
    {
        bCanEnterTravelMode = true;
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlayerPlaneMovementComponent::ResetEnteringTravelMode, 
            TimeBetweenDoubleThrustInputToEnterTravelMode, false);
        return;
    }

    if(bCanEnterTravelMode == false)
    {
        TravelModeThurstActivations = 0;
        if(bIsInTravelMode)
        {
            DeactivateTravelMode();
        }
    }

}


void UPlayerPlaneMovementComponent::ThrustInput(const FInputActionValue& Value)
{
    Super::ThrustInput(Value.GetMagnitude());
    

    if(Value.GetMagnitude() == 0.f)
    {
        ReleasedThrustInput();
    }

    if(bIsInTravelMode)
    {
        return;
    }
    
    if(Value.GetMagnitude() == 1.f)
    {
        if(TravelModeThurstActivations == 0 || bCanEnterTravelMode)
        {
            TravelModeThurstActivations += 1;
        }

        if(TravelModeThurstActivations == 2)
        {
            bCanEnterTravelMode = false;
            ActivateTravelMode();
        }
    }
    


}

void UPlayerPlaneMovementComponent::Thrusting(float InputVal)
{
    Super::Thrusting(InputVal);
}

void UPlayerPlaneMovementComponent::ActivateTravelMode()
{
    OnTravelModeActivate.Broadcast();
    bIsInTravelMode = true;
    AirControl = AirControl * TravelModeAircontrolMultiplier;
    MaxAcceleration = MaxAcceleration *  TravelModeMaxAccelerationMultiplier;
    MaxThrustUpAcceleration = MaxThrustUpAcceleration *  TravelModeMaxAccelerationMultiplier;
}


void UPlayerPlaneMovementComponent::DeactivateTravelMode()
{
    OnTravelModeDeactivate.Broadcast();
    AirControl = AirControl / TravelModeAircontrolMultiplier;
    MaxAcceleration = MaxAcceleration / TravelModeMaxAccelerationMultiplier; 
    MaxThrustUpAcceleration = MaxThrustUpAcceleration / TravelModeMaxAccelerationMultiplier ;
    bIsInTravelMode = false;
}
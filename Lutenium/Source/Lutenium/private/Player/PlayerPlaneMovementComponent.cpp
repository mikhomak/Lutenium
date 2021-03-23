#include "Player/PlayerPlaneMovementComponent.h"
#include "Player/PlayerPawn.h"
#include "Player/MovementEffect/PlayerEmpMovementEffect.h"
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


void UPlayerPlaneMovementComponent::HasDotChangedEventCaller(const float fNewDot)
{
	PlayerPawn->DotHasChange(fNewDot);
}

void UPlayerPlaneMovementComponent::OnKickInAccelerationEventCaller()
{
    PlayerPawn->OnKickInAccelerationEvent();
}
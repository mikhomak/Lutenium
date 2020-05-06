#include "../public/PlaneMovementComponent.h"
#include "../public/PlayerPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Math/Vector.h"
#include "Components/SkeletalMeshComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include <EngineGlobals.h>
#include <Runtime/Engine/Classes/Engine/Engine.h>

UPlaneMovementComponent::UPlaneMovementComponent() {
    PrimaryComponentTick.bCanEverTick = true;


    ThrustAcceleration = 100.f;
    ThrustMaxSpeed = 8000.f;
    ThrustMinSpeed = 50.f;
    AirControl = 0.1f;
    CustomGravity = -600.f;
    YawnControl = 0.5f;
    PitchControl = 1.f;
    RollControl = 1.f;
	AerodynamicMultiplier = 0.4f;

}


void UPlaneMovementComponent::BeginPlay() {
    Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UPlaneMovementComponent::CalculateAcceleration, 0.05f,
                                           true);
}

void UPlaneMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction *ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    PlayerMesh->AddTorqueInDegrees(PlayerMesh->GetPhysicsAngularVelocityInDegrees() * -1f / 0.5f, FName(), true);
    AddThrust();
    AddGravityForce();
	CalculateAerodynamic();
}


void UPlaneMovementComponent::ThrustInput(float Val) {
    Thrusting(Val);
}

void UPlaneMovementComponent::PitchInput(float Val) {
    AddTorqueToThePlane(PlayerPawn->GetActorRightVector(), Val * PitchControl);
}

void UPlaneMovementComponent::YawnInput(float Val) {
    AddTorqueToThePlane(PlayerPawn->GetActorUpVector(), Val * YawnControl);
}

void UPlaneMovementComponent::RollInput(float Val) {
    AddTorqueToThePlane(PlayerPawn->GetActorForwardVector(), Val * RollControl);
}

void UPlaneMovementComponent::AddTorqueToThePlane(FVector Direction, float InputVal) {
    if (InputVal != 0) {
        FVector ZeroVector;
        FVector DirectionToTilt = FMath::Lerp(ZeroVector, Direction * InputVal * AirControl, 0.1f);
        PlayerMesh->AddTorque(DirectionToTilt, FName(), true);
    }
}

void UPlaneMovementComponent::Thrusting(float InputVal) {
    ThrustUp = InputVal == 0 ? false : InputVal > 0 ? true : false;
}

void UPlaneMovementComponent::AddThrust() {
    float Speed = FMath::Clamp(ThrustAcceleration * Acceleration, ThrustMinSpeed, ThrustMaxSpeed);
    FVector Velocity = FMath::Lerp(PlayerMesh->GetPhysicsLinearVelocity(), PlayerMesh->GetForwardVector() * Speed,
                                   0.014f);
    PlayerMesh->SetPhysicsLinearVelocity(Velocity, false, FName());
}

void UPlaneMovementComponent::CalculateAcceleration() {
    Acceleration += ThrustUp ? 1 : -1;
    Acceleration = FMath::Clamp(Acceleration, 0.f, 100.f);
}

void UPlaneMovementComponent::AddGravityForce() {
    PlayerMesh->AddForce(FVector(0, 0, CustomGravity), FName(), true);
}

void UPlaneMovementComponent::SetMesh(USkeletalMeshComponent *Mesh) {
    PlayerMesh = Mesh;
}

void UPlaneMovementComponent::SetPawn(APlayerPawn *Pawn) {
    PlayerPawn = Pawn;
}

void UPlaneMovementComponent::StopInput() {
    float Speed = FMath::Clamp(ThrustAcceleration * Acceleration, ThrustMinSpeed, ThrustMaxSpeed);
    //FVector Velocity = PlayerPawn->GetVelocity() + PlayerMesh->GetForwardVector() * (Speed / 5);
    PlayerMesh->SetPhysicsLinearVelocity(PlayerMesh->GetForwardVector() * (Speed / 5), true, FName());
}

void UPlaneMovementComponent::CalculateAerodynamic(){
	FVector Velocity = PlayerPawn->GetVelocity();
	FVector UpVector = PlayerPawn->GetActorUpVector();
	Velocity.Normalize();
	float DotProduct = FVector::DotProduct(UpVector, Velocity);
	if (DotProduct < 0) {
		PlayerMesh->AddTorque(Velocity * DotProduct * AerodynamicMultiplier, FName(), true);
	}
}
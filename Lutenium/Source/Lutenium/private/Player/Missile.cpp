// Fill out your copyright notice in the Description page of Project Settings.

#include "../../public/Player/Missile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AMissile::AMissile()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostPhysics;
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Capcule collider"));
    SphereComponent->SetSimulatePhysics(true);
    RootComponent = SphereComponent;

    MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Missile Mesh"));
    MissileMesh->AttachToComponent(SphereComponent, FAttachmentTransformRules::KeepWorldTransform);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovement->bRotationFollowsVelocity = true;

    Damage = 150.f;
    WavesLifeSpan = 20.f;
}

void AMissile::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(WavesLifeSpan);
}

void AMissile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMissile::SetTargetOrDirection(USceneComponent *Target, const FVector &ShootDirection)
{
    if (Target != nullptr)
    {
        ProjectileMovement->bIsHomingProjectile = true;
        ProjectileMovement->HomingTargetComponent = Target;
    }
    Direction = ShootDirection;
}

void AMissile::ThrowMissile(FVector ThrownDirection, float ForceAmount)
{
    ProjectileMovement->MaxSpeed = 0.f;
    SphereComponent->AddImpulse(ThrownDirection * ForceAmount, FName(), true);
    bIsDefected = true;
    FTimerHandle TimerHandle;
    //GetWorld()->GetTimerManager().SetTiemr()
}

void AMissile::DefectedMissileGravity()
{

}

void AMissile::DefectedMissileImpulse()
{

}
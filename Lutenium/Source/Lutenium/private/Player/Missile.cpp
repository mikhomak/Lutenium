// Fill out your copyright notice in the Description page of Project Settings.

#include "../../public/Player/Missile.h"
#include "../../public/Player/PlayerPawn.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/TimelineComponent.h"
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
    MissileLifeSpan = 20.f;
    if (Curve)
    {
        FOnTimelineFloat TimelineCallback;
        FOnTimelineEventStatic TimelineFinishedCallback;

        TimelineCallback.BindUFunction(this, FName("DefectedMissileGravity"));
        TimelineFinishedCallback.BindUFunction(this, {FName("DefectedMissileImpulse")});

        DefectedTimeline.AddInterpFloat(Curve, TimelineCallback);
        DefectedTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
    }
}

void AMissile::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(MissileLifeSpan);
}

void AMissile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    DefectedTimeline.TickTimeline(DeltaTime);
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
    DefectedTimeline.PlayFromStart();
    GotDefected();
}

void AMissile::DefectedMissileGravity()
{
    if(FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation()) < DistanceToThePlayerWhenTheDefectedMissileIsAboutToBlowUp)
    {
        FVector GravityDirection = GetActorLocation() - PlayerPawn->GetActorLocation();
        GravityDirection.Normalize();
        GravityDirection *= DefecteedGravityForceAmount;
        //ParentPawn->AddImpulse(GravityDirection, FName(), true);
        bIsPawnGravited = true;
    }
    else if(bIsPawnGravited)
    {
        bIsPawnGravited= false;
    }
}

void AMissile::DefectedMissileImpulse()
{
    if(FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation()) < DistanceToThePlayerWhenTheDefectedMissileIsAboutToBlowUp)
    {
        DefectedImpulse();
        FVector ImpulseDirection = PlayerPawn->GetActorLocation() - GetActorLocation();
        ImpulseDirection.Normalize();
        ImpulseDirection *= DefecteedImpulseForceAmount;
        PlayerPawn->GetBoxComponent()->AddImpulse(ImpulseDirection, FName(), true);
        Destroy();
    }
}
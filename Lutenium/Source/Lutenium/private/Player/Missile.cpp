// Fill out your copyright notice in the Description page of Project Settings.

#include "../../public/Player/Missile.h"
#include "../../public/Player/PlayerPawn.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"

AMissile::AMissile()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Capcule collider"));
    SphereComponent->SetSimulatePhysics(false);
    RootComponent = SphereComponent;

    MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Missile Mesh"));
    MissileMesh->AttachToComponent(SphereComponent, FAttachmentTransformRules::KeepWorldTransform);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovement->bRotationFollowsVelocity = true;

    NonTargetSpeed = 500.f;

    Damage = 150.f;
    ExplosionRadius = 2000.f;
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
    else
    {
        Direction = ShootDirection;
        Direction.Normalize();
        ProjectileMovement->Velocity = Direction * NonTargetSpeed;
    }
}

void AMissile::ThrowMissile(FVector ThrownDirection, float ForceAmount)
{
    // Enemy could throw the missile and it will explode against the player
    ProjectileMovement->MaxSpeed = 0.f;
    if(ThrownDirection.IsNormalized())
    {
        ThrownDirection.Normalize();
    }
    ProjectileMovement->Velocity = ThrownDirection;
    bIsDefected = true;
    DefectedTimeline.PlayFromStart();
    GotDefected();
}

void AMissile::DefectedMissileGravity()
{
    // While the missile is defected, attaching the player to it
    if(FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation()) < DistanceToThePlayerWhenTheDefectedMissileIsAboutToBlowUp)
    {
        FVector GravityDirection = GetActorLocation() - PlayerPawn->GetActorLocation();
        GravityDirection.Normalize();
        GravityDirection *= DefecteedGravityForceAmount;
        PlayerPawn->GetPlaneBox()->AddForce(GravityDirection, FName(), true);
        bIsPawnGravited = true;
    }
    else if(bIsPawnGravited)
    {
        bIsPawnGravited= false;
    }
}

void AMissile::DefectedMissileImpulse()
{
    // After some amount of time of being deflected, explodes if the player is close lel
    if(FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation()) < DistanceToThePlayerWhenTheDefectedMissileIsAboutToBlowUp)
    {
        DefectedImpulse();
        FVector ImpulseDirection = PlayerPawn->GetActorLocation() - GetActorLocation();
        ImpulseDirection.Normalize();
        ImpulseDirection *= DefecteedImpulseForceAmount;
        PlayerPawn->GetPlaneBox()->AddImpulse(ImpulseDirection, FName(), true);
        Destroy();
    }
}

void AMissile::Explode()
{
    OnExplode();
    // Spawn VFX in Bp!
    // Faster to do those shit in BP lol
    // Also bind this function to Overlap and OnDestroy event
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage,
                                        GetActorLocation(), ExplosionRadius,
                                        UDamageType::StaticClass(), TArray<AActor*>(),
                                        this, (AController*)GetOwner(),
                                        true, ECC_Visibility);
}
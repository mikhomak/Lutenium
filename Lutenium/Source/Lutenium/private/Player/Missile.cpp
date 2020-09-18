// Fill out your copyright notice in the Description page of Project Settings.


#include "../../public/Player/Missile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AMissile::AMissile()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostPhysics;
    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Capcule collider"));
    RootComponent = SphereComponent;


    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovement->InitialSpeed = InitialSpeed;
    ProjectileMovement->MaxSpeed = MaxSpeed;
    ProjectileMovement->bRotationFollowsVelocity = true;


    ExplosionRadius = 700.f;
    Damage = 150.f;
}

void AMissile::BeginPlay()
{
    Super::BeginPlay();
}

void AMissile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMissile::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    if (OtherActor && ParentPawn)
    {
        // Ignoring ourselves
        TArray<AActor*> IgnoredActors;
        IgnoredActors.Add(this);

        UGameplayStatics::ApplyRadialDamage(this, Damage,
                                            GetActorLocation(), ExplosionRadius,
                                            nullptr, IgnoredActors, this,
                                            ParentPawn->GetController(), true);
        Destroy();
    }
}

void AMissile::SetTargetOrDirection(USceneComponent* Target, const FVector& ShootDirection)
{
    if (Target != nullptr)
    {
        ProjectileMovement->bIsHomingProjectile = true;
        ProjectileMovement->HomingTargetComponent = Target;
    }
    Direction = ShootDirection;
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMissile::StartFlying, TimeBeforeFly, false);
    AfterInstantiate();
}


void AMissile::AfterInstantiate_Implementation()
{
}

void AMissile::BeginFlying_Implementation()
{
}

void AMissile::StartFlying()
{
    if (!ProjectileMovement->bIsHomingProjectile)
    {
        ProjectileMovement->Velocity = Direction * InitialSpeed;
    }
    //ProjectileMovement->Targe
    BeginFlying();
}

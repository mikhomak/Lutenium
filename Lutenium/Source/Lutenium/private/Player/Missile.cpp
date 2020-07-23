// Fill out your copyright notice in the Description page of Project Settings.


#include "../../public/Player/Missile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMissile::AMissile()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostPhysics;
    CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capcule collider"));
    RootComponent = CapsuleCollider;


    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CapsuleCollider);
    ProjectileMovementComponent->InitialSpeed = InitialSpeed;
    ProjectileMovementComponent->MaxSpeed = MaxSpeed;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.3f;
}

void AMissile::BeginPlay()
{
    Super::BeginPlay();
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMissile::StartFlying, TimeBeforeFly, false);
}

void AMissile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMissile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        OtherActor->TakeDamage(Damage, FDamageEvent(), nullptr, this);
        Destroy();
    }
}

void AMissile::SetDirection(const FVector& ShootDirection)
{
    Direction = ShootDirection;
}


void AMissile::AfterInstantiate_Implementation()
{
}

void AMissile::BeginFlying_Implementation()
{
}

void AMissile::StartFlying()
{
    ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

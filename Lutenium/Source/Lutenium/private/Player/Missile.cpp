// Fill out your copyright notice in the Description page of Project Settings.


#include "../../public/Player/Missile.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMissile::AMissile()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostPhysics;
    CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capcule collider"));
    FRotator Rotator;
    Rotator.Pitch=90.f;
    CapsuleCollider->SetWorldRotation(Rotator);
    RootComponent = CapsuleCollider;


    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovement->SetUpdatedComponent(CapsuleCollider);
    ProjectileMovement->InitialSpeed = InitialSpeed;
    ProjectileMovement->MaxSpeed = MaxSpeed;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 0.3f;
}

void AMissile::BeginPlay()
{
    Super::BeginPlay();

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
    ProjectileMovement->Velocity = Direction * InitialSpeed;
    BeginFlying();
}

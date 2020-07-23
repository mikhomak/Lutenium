// Fill out your copyright notice in the Description page of Project Settings.


#include "../../public/Player/Missile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AMissile::AMissile()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostPhysics;
    CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capcule collider"));
    RootComponent = CapsuleCollider;
    MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Missile Mesh"));
    MissileMesh->SetSimulatePhysics(false);
    MissileMesh->SetEnableGravity(true);


    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CapsuleCollider);
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
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

void AMissile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AMissile::SetPawn(APawn* NewPawn)
{
    Pawn = NewPawn;
}

void AMissile::AfterInstantiate_Implementation()
{
}

void AMissile::BeginFlying_Implementation()
{
}

void AMissile::StartFlying()
{
    bFlying = true;
    MissileMesh->SetSimulatePhysics(false);
    MissileMesh->SetEnableGravity(false);
    MissileMesh->SetPhysicsLinearVelocity(MissileMesh->GetForwardVector() * Speed);
}

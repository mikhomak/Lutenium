// Fill out your copyright notice in the Description page of Project Settings.


#include "../../public/Player/Missile.h"
#include "Components/StaticMeshComponent.h"

AMissile::AMissile()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PostPhysics;
    MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Missile Mesh"));
    MissileMesh->SetSimulatePhysics(false);
    MissileMesh->SetEnableGravity(true);
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

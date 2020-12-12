#include "Monster/Weapons/Scream.h"
#include "Player/PlayerPawn.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/MovementEffect/DragMovementEffect.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

AScream::AScream()
{
    MainSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Main Sphere"));
    RootComponent = MainSphereComp;

    WaveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wave Mesh"));
    WaveMesh->AttachToComponent(MainSphereComp, FAttachmentTransformRules::KeepWorldTransform);
    WaveMesh->SetGenerateOverlapEvents(true);

    WavesLifeSpan = 10.f;
    ScalingSpeed = 1.01f;
    ScalingMultiplier = FVector(1.f, 1.f, 0.f);

    ForceImpact = 9000.f;
    DragForce = 90000.f;
}

void AScream::BeginPlay()
{
    Super::BeginPlay();
    /* Add overlap function calls in blueprints! */
    SetLifeSpan(WavesLifeSpan);
}

void AScream::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const FVector FirstWaveCurrentScale = WaveMesh->GetComponentScale();
    const FVector FirstWaveNewScale = FirstWaveCurrentScale + ScalingMultiplier * ScalingSpeed;
    WaveMesh->SetWorldScale3D(FirstWaveNewScale);
}

void AScream::WaveOverlap(AActor* OtherActor)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
        FVector DragDirection = PlayerPawn->GetActorLocation() - GetActorLocation();
        DragDirection.Normalize();
        // Dragging or impulsing the player aways lel
        if(bDragOrImpulse)
        {
            PlayerPawn->GetPlaneMovement()->DragMovementEffect->Activate(DragForce, DragDirection);
        }
        else
        {
            PlayerPawn->GetPlaneBox()->AddImpulse(DragDirection * 9000.f, FName(), true);
            WaveMesh->SetCollisionProfileName(TEXT("IgnoreAll"));
        }
    }
}


void AScream::WaveOverlapEnd(AActor* OtherActor)
{
    if(bDragOrImpulse)
    {
        APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
        if (PlayerPawn)
        {
            PlayerPawn->GetPlaneMovement()->DragMovementEffect->Deactivate();
        }
    }
}


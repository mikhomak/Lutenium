#include "../../../public/Monster/Weapons/Scream.h"
#include "../../../public/Player/PlayerPawn.h"
#include "../../../public/Player/PlaneMovementComponent.h"
#include "../../../public/Player/MovementEffect/DragMovementEffect.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

AScream::AScream()
{
    MainSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Main Sphere"));
    RootComponent = MainSphereComp;

    FirstWaveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("First Wave"));
    FirstWaveMesh->AttachToComponent(MainSphereComp, FAttachmentTransformRules::KeepWorldTransform);
    FirstWaveMesh->SetGenerateOverlapEvents(true);
    SecondWaveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Second Wave"));
    SecondWaveMesh->AttachToComponent(MainSphereComp, FAttachmentTransformRules::KeepWorldTransform);
    SecondWaveMesh->SetGenerateOverlapEvents(true);
    WavesLifeSpan = 10.f;
    ScalingSpeed = 1.01f;
    SecondFaveDelay = 1.f;
    ScalingMultiplier = FVector(1.f, 1.f, 0.f);

    FirstWaveForceImpact = 9000.f;
    FirstWaveDragForce = 90000.f;
}

void AScream::BeginPlay()
{
    Super::BeginPlay();
    /* Add overlap function calls in blueprints! */
    SetLifeSpan(WavesLifeSpan);
    FTimerHandle SecondWaveDelayTimerHandler;
    GetWorld()->GetTimerManager().SetTimer(SecondWaveDelayTimerHandler, this, &AScream::SecondWaveDelayStart,
                                           SecondFaveDelay,
                                           false);
}

void AScream::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const FVector FirstWaveCurrentScale = FirstWaveMesh->GetComponentScale();
    const FVector FirstWaveNewScale = FirstWaveCurrentScale + ScalingMultiplier * ScalingSpeed;
    FirstWaveMesh->SetWorldScale3D(FirstWaveNewScale);
    if (DidSecondWaveStart)
    {
        const FVector SecondWaveCurrentScale = SecondWaveMesh->GetComponentScale();
        const FVector SecondWaveNewScale = SecondWaveCurrentScale + ScalingMultiplier * ScalingSpeed;
        SecondWaveMesh->SetWorldScale3D(SecondWaveNewScale);
    }
}

void AScream::FirstWaveOverlap(
    UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
        FVector DragDirection = PlayerPawn->GetActorLocation() - GetActorLocation();
        DragDirection.Normalize();
        PlayerPawn->GetPlaneMovement()->DragMovementEffect->Activate(FirstWaveDragForce, DragDirection);
    }
}

void AScream::SecondWaveOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                bool bFromSweep, const FHitResult& SweepResult)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
        FVector DragDirection = PlayerPawn->GetActorLocation() - GetActorLocation();
        DragDirection.Normalize();
        PlayerPawn->GetPlaneBox()->AddImpulse(DragDirection * 9000.f, FName(), true);
        SecondWaveMesh->SetCollisionProfileName(TEXT("IgnoreAll"));
    }
}


void AScream::FirstWaveOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
        PlayerPawn->GetPlaneMovement()->DragMovementEffect->Deactivate();
    }
}

void AScream::SecondWaveOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AScream::SecondWaveDelayStart()
{
    DidSecondWaveStart = true;
}

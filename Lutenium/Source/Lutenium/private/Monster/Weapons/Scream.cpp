
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
#include "../../../public/Monster/Weapons/Scream.h"

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
}

void AScream::BeginPlay()
{
	Super::BeginPlay();
	/* Add overlap function calls in blueprints! */
	SetLifeSpan(WavesLifeSpan);
}

void AScream::Tick(float DeltaTime)
{
	const FVector FirstWaveCurrentScale = FirstWaveMesh->GetComponentScale();
	const FVector FirstWaveBiggerScale = FVector(FirstWaveCurrentScale.X, FirstWaveCurrentScale.Y,0 ) * ScalingSpeed;
	const FVector FirstWaveFinalScale = FMath::Lerp(FirstWaveCurrentScale, FirstWaveBiggerScale, 0.5f);
	FirstWaveMesh->SetWorldScale3D(FirstWaveFinalScale);
}

void AScream::FirstWaveOverlap(
	UPrimitiveComponent *OverlappedComp,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("1"))
}

void AScream::SecondWaveOverlap(
	UPrimitiveComponent *OverlappedComp,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("2"))
}

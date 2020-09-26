#include "../../../public/Monster/Weapons/Scream.h"
#include "../../../public/Player/PlayerPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Math/UnrealMathUtility.h"
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

	FirstWaveForceImpact=9000.f;
}

void AScream::BeginPlay()
{
	Super::BeginPlay();
	/* Add overlap function calls in blueprints! */
	SetLifeSpan(WavesLifeSpan);
	FTimerHandle SecondWaveDelayTimerHandler;
	GetWorld()->GetTimerManager().SetTimer(SecondWaveDelayTimerHandler, this, &AScream::SecondWaveDelayStart, SecondFaveDelay,
										   false);
}

void AScream::Tick(float DeltaTime)
{
	const FVector FirstWaveCurrentScale = FirstWaveMesh->GetComponentScale();
	const FVector FirstWaveNewScale = FVector(FirstWaveCurrentScale.X + ScalingSpeed, FirstWaveCurrentScale.Y + ScalingSpeed, 1);
	FirstWaveMesh->SetWorldScale3D(FirstWaveNewScale);
	if (DidSecondWaveStart)
	{
		const FVector SecondWaveCurrentScale = SecondWaveMesh->GetComponentScale();
		const FVector SecondWaveNewScale = FVector(SecondWaveCurrentScale.X + ScalingSpeed, SecondWaveCurrentScale.Y + ScalingSpeed, 1);
		SecondWaveMesh->SetWorldScale3D(SecondWaveNewScale);
	}
}

void AScream::FirstWaveOverlap(
	UPrimitiveComponent *OverlappedComp,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
	if(PlayerPawn)
	{
		PlayerPawn->GetPlaneMesh()->AddForce(SweepResult.ImpactNormal * FirstWaveForceImpact);
		FirstWaveMesh->SetCollisionProfileName(TEXT("IgnoreAll"));
	}
}

void AScream::SecondWaveOverlap(
	UPrimitiveComponent *OverlappedComp,
	AActor *OtherActor,
	UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult &SweepResult)
{
	APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
	if(PlayerPawn)
	{
		SecondWaveMesh->SetCollisionProfileName(TEXT("IgnoreAll"));
	}
}

void AScream::SecondWaveDelayStart()
{
	DidSecondWaveStart = true;
}
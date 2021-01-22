#include "Monster/Weapons/Spells/SmokeSpell.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"


ASmokeSpell::ASmokeSpell()
{
	PrimaryActorTick.bCanEverTick = true;

	SmokeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectlie mesh"));
    RootComponent = SmokeMesh;

	LifeTime = 120.f; /* 2 mins */
}

void ASmokeSpell::BeginPlay()
{
	Super::BeginPlay();
    SetLifeSpan(LifeTime);

}

void ASmokeSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


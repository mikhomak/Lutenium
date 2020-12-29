#include "Monster/Weapons/Spells/PowerProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"


APowerProjectile::APowerProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	MainSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Main Sphere"));
    RootComponent = MainSphereComp;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectlie mesh"));
    ProjectileMesh->AttachToComponent(MainSphereComp, FAttachmentTransformRules::KeepWorldTransform);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile movement"));
    ProjectileMovement->bRotationFollowsVelocity = true;

	LifeTime=15.f;
}

void APowerProjectile::BeginPlay()
{
	Super::BeginPlay();
    SetLifeSpan(LifeTime);

}

void APowerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


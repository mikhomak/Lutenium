
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "../../../public/Monster/Weapons/Scream.h"

AScream::AScream()
{
	MainSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Main Sphere"));
	RootComponent = MainSphereComp;

	FirstSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("First Sphere"));
	FirstSphereComp->AttachTo(MainSphereComp);
	SecondSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Second Sphere"));
	SecondSphereComp->AttachTo(MainSphereComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(MainSphereComp);
}


void AScream::BeginPlay()
{
	Super::BeginPlay();

}

void AScream::Tick(float DeltaTime)
{
}







#include "../../../public/Monster/Weapons/Scream.h"

UScream::UScream()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UScream::BeginPlay()
{
	Super::BeginPlay();

	
}


void UScream::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}



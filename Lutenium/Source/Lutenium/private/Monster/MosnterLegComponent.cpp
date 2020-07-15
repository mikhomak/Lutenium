

#include "../../public/Monster/MosnterLegComponent.h"

UMosnterLegComponent::UMosnterLegComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UMosnterLegComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


void UMosnterLegComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UMosnterLegComponent::RaycastLeg()
{

	FHitResult HitResult;
	const FVector RaycastEndLocation = RaycastLocation - (FVector::DownVector * EnemyMonsterPawn.RaycastDownLength);

	GetWorld()->LineTraceSingleByChannel(
        HitResult,
        RaycastLocation,
        RaycastEndLocation,
        ECollisionChannel::ECC_WorldDynamic);

	FVector HitLocation = HitResult.bBlockingHit ? HitResult.ImpactPoint : FVector();

	if (FVector::Distance(CurrentPosition, HitLocation) >= EnemyMonsterPawn.DistanceBetweenLegsToMove)
	{
        
	}
}

void UMosnterLegComponent::TimelineCallback()
{
}

void UMosnterLegComponent::TimelineFinish()
{
}


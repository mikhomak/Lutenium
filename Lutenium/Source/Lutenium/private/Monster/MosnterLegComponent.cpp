#include "../../public/Monster/MosnterLegComponent.h"

#include "Profiler/Private/Widgets/SDataGraph.h"

UMosnterLegComponent::UMosnterLegComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}


void UMosnterLegComponent::BeginPlay()
{
    Super::BeginPlay();
}


void UMosnterLegComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (Curve)
    {
        FOnTimelineFloat TimelineCallback;
        FOnTimelineEventStatic TimelineFinishedCallback;

        TimelineCallback.BindUFunction(this, FName("TimelineCallback"));
        TimelineFinishedCallback.BindUFunction(this, FName("TimelineFinish"));

        LegTimeline.AddInterpFloat(Curve, TimelineCallback);
        LegTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
    }
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

    const FVector HitLocation = HitResult.bBlockingHit ? HitResult.ImpactPoint : FVector();

    if (FVector::Distance(CurrentPosition, HitLocation) >= EnemyMonsterPawn.DistanceBetweenLegsToMove && bCanMove)
    {
        LegTimeline.Play();
        HighPointBetweenSteps = (HitLocation - CurrentPosition).Z + EnemyMonsterPawn.BetweenStepHigh;
        bCanMove = false;
    }
}

void UMosnterLegComponent::TimelineCallback()
{
    TimelineValue = LegTimeline.GetPlaybackPosition();
    
    CurrentFloatTimelineValue = Curve->GetFloatValue(TimelineValue);
    
}

void UMosnterLegComponent::TimelineFinish()
{
    bCanMove = true;
}

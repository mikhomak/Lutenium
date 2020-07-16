#include "../../public/Monster/MosnterLegComponent.h"

#include "Profiler/Private/Widgets/SDataGraph.h"

UMosnterLegComponent::UMosnterLegComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostPhysics;
    

    Curve->GetTimeRange(MinTimeCurve, MaxTimeCurve);
}


void UMosnterLegComponent::BeginPlay()
{
    Super::BeginPlay();
    Curve = Curve == nullptr ? EnemyMonsterPawn->LegFloatCurve : *Curve;
    DistanceBetweenLegsToMove = EnemyMonsterPawn->DistanceBetweenLegsToMove;
    RaycastDownLength = EnemyMonsterPawn->RaycastDownLength;
    HighestPoint = EnemyMonsterPawn->HighestPoint;
    LerpValue = EnemyMonsterPawn->LerpValue;
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
    const FVector RaycastEndLocation = RaycastLocation - (FVector::DownVector * RaycastDownLength);

    GetWorld()->LineTraceSingleByChannel(
        HitResult,
        RaycastLocation,
        RaycastEndLocation,
        ECollisionChannel::ECC_WorldDynamic);

    const FVector HitLocation = HitResult.bBlockingHit ? HitResult.ImpactPoint : FVector();

    if (HitResult.bBlockingHit &&
        FVector::Distance(CurrentPosition, HitLocation) >= DistanceBetweenLegsToMove &&
        bCanMove)
    {
        StartPosition = CurrentPosition;
        FinishPosition = HitLocation;
        HighPointBetweenSteps = (HitLocation - CurrentPosition).Z + HighestPoint;
        LowestPointBetweenSteps = FMath::Min(HitLocation.Z, CurrentPosition.Z);
        bCanMove = false;
        LegTimeline.Play();
    }
}

void UMosnterLegComponent::TimelineCallback()
{
    TimelineValue = LegTimeline.GetPlaybackPosition();

    CurrentFloatTimelineValue = Curve->GetFloatValue(TimelineValue);


    const float YValue = FMath::GetMappedRangeValueUnclamped(
        FVector2D(0, 1),
        FVector2D(LowestPointBetweenSteps, HighPointBetweenSteps),
        CurrentFloatTimelineValue);

    CurrentPosition = FMath::Lerp(
        CurrentPosition,
        FVector(GetCurrentValueForAxis(true), YValue, GetCurrentValueForAxis(false))
        LerpValue);
}

void UMosnterLegComponent::TimelineFinish()
{
    bCanMove = true;
}

float UMosnterLegComponent::GetCurrentValueForAxis(const bool IsX)
{
    const float MinValue = IsX ? StartPosition.X : StartPosition.Y;
    const float MaxValue = IsX ? FinishPosition.X : FinishPosition.Y;
    return FMath::GetMappedRangeValueClamped(
        FVector2D(MinTimeCurve, MaxTimeCurve),
        FVector2D(MinValue, MaxValue),
        CurrentFloatTimelineValue);
}


void UMosnterLegComponent::SetRaycastLocation(const FVector& Location)
{
    RaycastLocation = Location;
}

FVector UMosnterLegComponent::GeCurrentPosition() const
{
    return CurrentPosition;
}

void UMosnterLegComponent::SetEnemyMonsterPawn(AEnemyMonsterPawn* MonsterPawn)
{
    EnemyMonsterPawn = MonsterPawn;
}

#include "../../public/Monster/MonsterLegComponent.h"
#include "../../public/Monster/EnemyMonsterPawn.h"

UMonsterLegComponent::UMonsterLegComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PostPhysics;
    

}


void UMonsterLegComponent::BeginPlay()
{
    Super::BeginPlay();
    Curve = Curve == nullptr ? EnemyMonsterPawn->LegFloatCurve : Curve;
    DistanceBetweenLegsToMove = EnemyMonsterPawn->DistanceBetweenLegsToMove;
    RaycastDownLength = EnemyMonsterPawn->RaycastDownLength;
    HighestPoint = EnemyMonsterPawn->HighestPoint;
    LerpValue = EnemyMonsterPawn->LerpValue;
    Curve->GetTimeRange(MinTimeCurve, MaxTimeCurve);

}


void UMonsterLegComponent::TickComponent(float DeltaTime, ELevelTick TickType,
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


void UMonsterLegComponent::RaycastLeg()
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

void UMonsterLegComponent::TimelineCallback()
{
    TimelineValue = LegTimeline.GetPlaybackPosition();

    CurrentFloatTimelineValue = Curve->GetFloatValue(TimelineValue);


    const float YValue = FMath::GetMappedRangeValueUnclamped(
        FVector2D(0, 1),
        FVector2D(LowestPointBetweenSteps, HighPointBetweenSteps),
        CurrentFloatTimelineValue);

    CurrentPosition = FMath::Lerp(
        CurrentPosition,
        FVector(GetCurrentValueForAxis(true), YValue, GetCurrentValueForAxis(false)),
        LerpValue);
}

void UMonsterLegComponent::TimelineFinish()
{
    bCanMove = true;
}

float UMonsterLegComponent::GetCurrentValueForAxis(const bool IsX)
{
    const float MinValue = IsX ? StartPosition.X : StartPosition.Y;
    const float MaxValue = IsX ? FinishPosition.X : FinishPosition.Y;
    return FMath::GetMappedRangeValueClamped(
        FVector2D(MinTimeCurve, MaxTimeCurve),
        FVector2D(MinValue, MaxValue),
        CurrentFloatTimelineValue);
}


void UMonsterLegComponent::SetRaycastLocation(const FVector& Location)
{
    RaycastLocation = Location;
}

FVector UMonsterLegComponent::GetCurrentPosition() const
{
    return CurrentPosition;
}

void UMonsterLegComponent::SetEnemyMonsterPawn(AEnemyMonsterPawn* MonsterPawn)
{
    EnemyMonsterPawn = MonsterPawn;
}

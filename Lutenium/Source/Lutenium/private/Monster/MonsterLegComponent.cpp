#include "../../public/Monster/MonsterLegComponent.h"
#include "../../public/Monster/EnemyMonsterPawn.h"
#include "Components/TimelineComponent.h"

UMonsterLegComponent::UMonsterLegComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bCanMove = true;
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

    if (Curve)
    {
        FOnTimelineFloat TimelineCallback;
        FOnTimelineEventStatic TimelineFinishedCallback;

        TimelineCallback.BindUFunction(this, FName("TimelineCallback"));
        TimelineFinishedCallback.BindUFunction(this, {FName("TimelineFinished")});

        LegTimeline.AddInterpFloat(Curve, TimelineCallback);
        LegTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
    }
}


void UMonsterLegComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    LegTimeline.TickTimeline(DeltaTime);
    RaycastLeg();
}


void UMonsterLegComponent::RaycastLeg()
{
    if (!bCanMove)
    {
        return;
    }

    FHitResult HitResult;
    const FVector RaycastEndLocation = RaycastLocation + (FVector::DownVector * RaycastDownLength);

    GetWorld()->LineTraceSingleByChannel(
        HitResult,
        RaycastLocation,
        RaycastEndLocation,
        ECollisionChannel::ECC_WorldStatic);

    const FVector HitLocation = HitResult.bBlockingHit ? HitResult.ImpactPoint : FVector();
    if (HitResult.bBlockingHit &&
        FVector::Distance(CurrentPosition, HitLocation) >= DistanceBetweenLegsToMove)
    {
        StartPosition = CurrentPosition;
        FinishPosition = HitLocation;
        HighPointBetweenSteps = FMath::Max(HitLocation.Z, CurrentPosition.Z) + HighestPoint;
        LowestPointBetweenSteps = FMath::Min(HitLocation.Z, CurrentPosition.Z);
        bCanMove = false;
        bHasReachedHighestPoint = false;
        LegTimeline.PlayFromStart();
    }
}


void UMonsterLegComponent::TimelineCallback()
{
    TimelineValue = LegTimeline.GetPlaybackPosition();

    CurrentFloatTimelineValue = Curve->GetFloatValue(TimelineValue);


    float ZValue;
    CalculateZValue(ZValue);

    CurrentPosition = FMath::Lerp(
        CurrentPosition,
        FVector(GetCurrentValueForAxis(true), GetCurrentValueForAxis(false), ZValue),
        LerpValue);
    UE_LOG(LogTemp, Warning, TEXT("CALCULAGTED THE POSITION"));
}

void UMonsterLegComponent::TimelineFinished()
{
    UE_LOG(LogTemp, Warning, TEXT("TIMELINE IS FINISHED MY GUY"));

    bCanMove = true;
}

float UMonsterLegComponent::GetCurrentValueForAxis(const bool IsX)
{
    const float MinValue = IsX ? StartPosition.X : StartPosition.Y;
    const float MaxValue = IsX ? FinishPosition.X : FinishPosition.Y;
    return FMath::GetMappedRangeValueClamped(
        FVector2D(0, 1),
        FVector2D(MinValue, MaxValue),
        TimelineValue);
}


void UMonsterLegComponent::CalculateZValue(float& ZValue)
{
    ZValue = FMath::GetMappedRangeValueUnclamped(
        FVector2D(0, 1),
        FVector2D(LowestPointBetweenSteps, HighPointBetweenSteps),
        CurrentFloatTimelineValue);
    FString one = FString::SanitizeFloat(LowestPointBetweenSteps);
    FString two = FString::SanitizeFloat(HighPointBetweenSteps);
    GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *one);
    GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *two);
    if (ZValue > HighPointBetweenSteps - 50.f)
    {
        bHasReachedHighestPoint = true;
    }
    if (bHasReachedHighestPoint && ZValue < FinishPosition.Z)
    {
        ZValue = FinishPosition.Z;
    }
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

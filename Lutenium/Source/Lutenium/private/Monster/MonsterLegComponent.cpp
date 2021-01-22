#include "Monster/MonsterLegComponent.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Components/TimelineComponent.h"

UMonsterLegComponent::UMonsterLegComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    bMoving = false;
}


void UMonsterLegComponent::BeginPlay()
{
    Super::BeginPlay();
    Curve = Curve == nullptr ? EnemyMonsterPawn->LegFloatCurve : Curve;
    DistanceBetweenLegsToMove = EnemyMonsterPawn->DistanceBetweenLegsToMove;
    RaycastDownLength = EnemyMonsterPawn->RaycastDownLength;
    AddedYValue = EnemyMonsterPawn->HighestPoint;
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

    /* Sets default location for each leg no matter if it can move or not */
    FHitResult HitResult;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(EnemyMonsterPawn);
    FVector RaycastLocation = MonsterMesh->GetSocketLocation(RaycastSocket); /* Gets location of the raycast socket */
    FVector RaycastEndLocation = RaycastLocation + (FVector::DownVector * RaycastDownLength); /* Downvector from this socket  */
    FVector DownRaycast = RaycastJoint(RaycastLocation,
                                        RaycastEndLocation,
                                        HitResult, CollisionParams);
    if(HitResult.bBlockingHit)
    {
        CurrentPosition = DownRaycast;
    }
}


void UMonsterLegComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    LegTimeline.TickTimeline(DeltaTime);
    RaycastLeg();
}


void UMonsterLegComponent::StartMovingLeg(const FVector HitLocation)
{
    StartPosition = CurrentPosition;
    FinishPosition = HitLocation;

    // If start position is higher than the finish position, then we decrease the added Y Value to the highest point ( to make the leg go like a parabola) 
    const float AddedYValueCalculated = StartPosition.Z > FinishPosition.Z ? AddedYValue / 3 : AddedYValue;

    HighPointBetweenSteps = FMath::Max(HitLocation.Z, CurrentPosition.Z) + AddedYValueCalculated;
    LowestPointBetweenSteps = FMath::Min(HitLocation.Z, CurrentPosition.Z);
    bMoving = true;
    bHasReachedHighestPoint = false;
    LegTimeline.PlayFromStart();
}


void UMonsterLegComponent::RaycastLeg()
{
    if (bMoving || !bCanMove)
    {
        return;
    }

    /* Raycasting only if the socket has moved too far away from the current leg position */
    FVector RaycastLocation = MonsterMesh->GetSocketLocation(RaycastSocket);
    FVector RaycastEndLocation = RaycastLocation + (FVector::DownVector * RaycastDownLength);
    const float DistanceBetweenCurrentPosAndPrevious = FVector::Distance(
        CurrentPosition, FVector(RaycastLocation.X, RaycastLocation.Y, CurrentPosition.Z));


    if (DistanceBetweenCurrentPosAndPrevious >= DistanceBetweenLegsToMove)
    {
        FHitResult HitResult;
        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(EnemyMonsterPawn);
        FVector RaycastResultLocation;
        FVector SecondJoint = MonsterMesh->GetSocketLocation(SecondJointSocket);

        FVector DownRaycast = RaycastJoint(RaycastLocation,
                                           RaycastEndLocation,
                                           HitResult, CollisionParams);

        /* If the position wasn't found, do nothing */
        if (HitResult.bBlockingHit)
        {
            /* Check if there is any obstacles from the second joint to the finish position */
            RaycastResultLocation = RaycastJoint(SecondJoint,
                                                 DownRaycast,
                                                 HitResult, CollisionParams);
            /* If there was an obstacle, then move the leg to the impact location with that obstacle */
            if (HitResult.bBlockingHit)
            {
                StartMovingLeg(RaycastResultLocation);
                return;
            }
            /* If no obstacles were found, then move the leg to the finish position */
            StartMovingLeg(DownRaycast);
        }
    }
}

FVector UMonsterLegComponent::RaycastJoint(FVector& StartPos, FVector& EndPos, FHitResult& HitResult,
                                           FCollisionQueryParams& CollisionParams)
{
    GetWorld()->LineTraceSingleByChannel(
        HitResult,
        StartPos,
        EndPos,
        ECollisionChannel::ECC_WorldStatic,
        CollisionParams);
    return HitResult.ImpactPoint;
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
}

void UMonsterLegComponent::TimelineFinished()
{
    bMoving = false;
    EnemyMonsterPawn->LegHasMovedEventCaller(MonsterLegType);
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

    if (ZValue > HighPointBetweenSteps - 50.f)
    {
        // When leg has reached  the highest point between two legs
        bHasReachedHighestPoint = true;
    }
    if (bHasReachedHighestPoint && ZValue < FinishPosition.Z)
    {
        ZValue = FinishPosition.Z;
        // When leg has is has reached the highest point but finish point is higher, making sure that the leg is staying on that point 
    }
    if (!bHasReachedHighestPoint &&
        ZValue < StartPosition.Z)
    {
        // Do not move if thew leg is below the lowest position until that positions is reached
        ZValue = StartPosition.Z;
    }
}

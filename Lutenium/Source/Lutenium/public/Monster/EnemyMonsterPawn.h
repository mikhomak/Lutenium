#pragma once

#include "CoreMinimal.h"
#include "MonsterLeg.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Pawn.h"
#include "EnemyMonsterPawn.generated.h"

UCLASS()
class LUTENIUM_API AEnemyMonsterPawn : public APawn
{
    GENERATED_BODY()

    UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent* MonsterMesh;

public:
    AEnemyMonsterPawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category="Legs movement")
    UCurveFloat* LegFloatCurve;

    UPROPERTY(EditAnywhere, Category="Legs movement")
    float DistanceBetweenLegsToMove;

    UPROPERTY(EditAnywhere, Category="Legs movement")
    float RaycastDownLength;

    UPROPERTY(EditAnywhere, Category="Legs movement")
    struct FVector RaycastRearLeftPosition;
    UPROPERTY(EditAnywhere, Category="Legs movement")
    struct FVector RaycastRearRightPosition;
    UPROPERTY(EditAnywhere, Category="Legs movement")
    struct FVector RaycastFrontRightPosition;
    UPROPERTY(EditAnywhere, Category="Legs movement")
    struct FVector RaycastFrontLeftPosition;

    UFUNCTION()
    FVector CalculateLegPosition(const EMonsterLeg Leg);

    UFUNCTION()
    FVector GetLegPosition(const EMonsterLeg Leg) const;

private:


    struct FVector CurrentRearLeftPosition;
    struct FVector CurrentRearRightPosition;
    struct FVector CurrentFrontRightPosition;
    struct FVector CurrentFrontLeftPosition;

    struct FTimeline RearLeftLegTimeline;
    struct FTimeline RearRightLegTimeline;
    struct FTimeline FrontRightLegTimeline;
    struct FTimeline FrontLeftLegTimeline;
};

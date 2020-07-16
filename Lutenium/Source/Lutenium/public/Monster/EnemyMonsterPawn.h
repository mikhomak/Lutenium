#pragma once

#include "CoreMinimal.h"
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
    float BetweenStepHigh;
};

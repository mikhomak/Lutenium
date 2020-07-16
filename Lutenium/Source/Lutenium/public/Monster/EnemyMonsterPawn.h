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

    UPROPERTY(EditDefaultsOnly, Category="Legs movement")
    UCurveFloat* LegFloatCurve;

    UPROPERTY(EditDefaultsOnly, Category="Legs movement")
    float DistanceBetweenLegsToMove;

    UPROPERTY(EditDefaultsOnly, Category="Legs movement")
    float RaycastDownLength;

    UPROPERTY(EditDefaultsOnly, Category="Legs movement")
    float HighestPoint;

    UPROPERTY(EditDefaultsOnly, Category="Legs movement")
    float LerpValue;

    UPROPERTY(EditDefaultsOnly, Category="Legs")
    class UMonsterLegComponent* RearLeftLeg;

    UPROPERTY(EditDefaultsOnly, Category="Legs")
    class UMonsterLegComponent* RearRightLeg;

    UPROPERTY(EditDefaultsOnly, Category="Legs")
    class UMonsterLegComponent* FrontLeftLeg;

    UPROPERTY(EditDefaultsOnly, Category="Legs")
    class UMonsterLegComponent* FrontRightLeg;

};

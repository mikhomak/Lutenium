#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include  "MonsterLeg.h"
#include "EnemyMonsterPawn.generated.h"

UCLASS()
class LUTENIUM_API AEnemyMonsterPawn : public APawn
{
    GENERATED_BODY()

    UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent* MonsterMesh;

    UPROPERTY(Category="Legs",VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UMonsterLegComponent* RearLeftLeg;

    UPROPERTY(Category="Legs",VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UMonsterLegComponent* RearRightLeg;

    UPROPERTY(Category="Legs",VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UMonsterLegComponent* FrontLeftLeg;

    UPROPERTY(Category="Legs",VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UMonsterLegComponent* FrontRightLeg;

public:
    AEnemyMonsterPawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditDefaultsOnly, Category="Legs movement")
    float DistanceBetweenLegsToMove;

    UPROPERTY(EditDefaultsOnly, Category="Legs movement")
    float RaycastDownLength;

    UPROPERTY(EditDefaultsOnly, Category="Legs movement")
    float HighestPoint;

    UPROPERTY(EditDefaultsOnly, Category="Legs movement")
    float LerpValue;

    UPROPERTY(EditDefaultsOnly, Category="Legs movement")
    UCurveFloat* LegFloatCurve;

    UFUNCTION(BlueprintCallable)
    FVector GetLegLocation(EMonsterLeg Leg) const;

    void LegHasMovedEventCaller(EMonsterLeg MonsterLeg);

    UFUNCTION(BlueprintImplementableEvent)
    void LegHasMoved(EMonsterLeg Leg);

    UFUNCTION(BlueprintImplementableEvent)
    void MissileCollide(const FVector& HitLocation, const FVector& NormalizedDirection, const float DamageApplied);
    
    virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:

    void ToggleWhatLegsShouldMove(bool Left) const;
};
    
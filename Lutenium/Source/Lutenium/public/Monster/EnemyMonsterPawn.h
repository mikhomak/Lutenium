#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include  "MonsterLeg.h"
#include "Components/TimelineComponent.h"
#include "EnemyMonsterPawn.generated.h"


UCLASS()
class LUTENIUM_API AEnemyMonsterPawn : public APawn
{
    GENERATED_BODY()

    UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent* MonsterMesh;

public:

    AEnemyMonsterPawn();

    virtual void Tick(float DeltaTime) override;

    // ------------------------------------------------------------------
    // Legs
    // ------------------------------------------------------------------

    UPROPERTY(Category="Legs",VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UMonsterLegComponent* RearLeftLeg;

    UPROPERTY(Category="Legs",VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UMonsterLegComponent* RearRightLeg;

    UPROPERTY(Category="Legs",VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UMonsterLegComponent* FrontLeftLeg;

    UPROPERTY(Category="Legs",VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UMonsterLegComponent* FrontRightLeg;

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

    // ------------------------------------------------------------------
    // Damage
    // ------------------------------------------------------------------

    UFUNCTION(BlueprintCallable)
    void DoScream();

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category="Weapons")
    class UMonsterWeapon* MonsterWeapon;

    UFUNCTION(BlueprintImplementableEvent)
    void MissileCollide(const FVector& HitLocation, const FVector& NormalizedDirection, const float DamageApplied);

    virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser) override;


    // ------------------------------------------------------------------
    // Body Movement
    // ------------------------------------------------------------------


    UFUNCTION()
    void BodyTimelineMovement();

    UFUNCTION()
    void BodyTimelineMovementFinish();


    // ------------------------------------------------------------------
    // AI
    // ------------------------------------------------------------------

    UPROPERTY(EditAnywhere, Category = "AI")
    class UBehaviorTree* BehaviorTree;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    class UPawnSensingComponent* PawnSensingComp;


protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnSeePlayer(APawn* Pawn);

    UFUNCTION()
    void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

private:


    // ------------------------------------------------------------------
    // BODY MOVEMENT WHEN THERE IS AN OVERLAP FROM THE BODY TO THE LEGS
    // ------------------------------------------------------------------

    void ToggleWhatLegsShouldMove(bool Left) const;

    struct FTimeline BodyTimeline;

    TArray<FName> TopSocketLocationNames;

    bool bBodyMoving;

    FName BodySocketName;

    void CheckBodyAltitudeDependingOnLegs();
};

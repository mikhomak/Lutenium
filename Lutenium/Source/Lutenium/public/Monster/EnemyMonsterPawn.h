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

public:

    AEnemyMonsterPawn();

    virtual void Tick(float DeltaTime) override;

    // ------------------------------------------------------------------
    // General
    // ------------------------------------------------------------------

    UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent* MonsterMesh;

    UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USphereComponent* SphereComponent;

    UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UFloatingPawnMovement* PawnMovement;

    // ------------------------------------------------------------------
    // Legs
    // ------------------------------------------------------------------

    UPROPERTY(Category="Legs",VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TArray<class UMonsterLegComponent*> Legs;

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

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class ASputnikMW* Sputnik;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class ASirenMW* Siren;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class ATrafficLightMW* TrafficLight;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class APipeMw* Pipe;

    UPROPERTY( EditDefaultsOnly, Category ="Weapons")
    FName SputnikSocketName;

    UPROPERTY( EditDefaultsOnly, Category ="Weapons")
    FName SirenSocketName;

    UFUNCTION(BlueprintImplementableEvent)
    void MissileCollide(const FVector& HitLocation, const FVector& NormalizedDirection, const float DamageApplied);

    virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser) override;


    // ------------------------------------------------------------------
    // Body Movement
    // ------------------------------------------------------------------

    UFUNCTION(BlueprintCallable)
    bool RaycastLegJoints();

    UPROPERTY(EditDefaultsOnly, Category = "Body")
    FName BodySocketName;

    UPROPERTY(EditDefaultsOnly, Category = "Body")
    FVector BodyPosition;

    UPROPERTY(EditDefaultsOnly, Category = "Body")
    float BodyUpMovementSpeed;

    UPROPERTY(BlueprintReadWrite, Category = "Body")
    bool bIsBodyMovingUp;

    UFUNCTION()
    void BodyTimelineMovement();

    UFUNCTION()
    void BodyTimelineMovementFinish();

    UPROPERTY(EditDefaultsOnly, Category="Body")
    UCurveFloat* BodyFloatCurve;

    struct FTimeline BodyTimeline;

    FORCEINLINE FVector GetCurrentBodyPosition() const { return BodyPosition; }


    // ------------------------------------------------------------------
    // AI
    // ------------------------------------------------------------------

    UPROPERTY(EditAnywhere, Category = "AI")
    class UBehaviorTree* BehaviorTree;


protected:
    virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

    void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;


private:

    // ------------------------------------------------------------------
    // BODY MOVEMENT WHEN THERE IS AN OVERLAP FROM THE BODY TO THE LEGS
    // ------------------------------------------------------------------

    void ToggleWhatLegsShouldMove(bool Left) const;

    TArray<FName> TopSocketLocationNames;

    bool bBodyMoving;

    void CheckBodyAltitudeDependingOnLegs();
};

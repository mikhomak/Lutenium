#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MonsterLeg.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
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

    UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly)
    class USkeletalMeshComponent* MonsterMesh;

    UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly)
    class USphereComponent* SphereComponent;

    UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly)
    class UFloatingPawnMovement* PawnMovement;

    // ------------------------------------------------------------------
    // Player behaviour
    // ------------------------------------------------------------------

    /** Collision to check if the player is inside the beam defense radius */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
    class USphereComponent* BeamDefenseSphere;

    /** Handles overlapping BeamDefenseSphere with the player */
    UFUNCTION(BlueprintCallable)
    void PlayerHasEnteredBeamDefense(class AActor* OverlapActor);

    /** Handles overlap exit BeamDefenseSphere with the player */
    UFUNCTION(BlueprintCallable)
    void PlayerHasExitdBeamDefense(class AActor* OverlapExitActor);

    // ------------------------------------------------------------------
    // Legs
    // ------------------------------------------------------------------

    UFUNCTION(BlueprintCallable)
    FVector GetLegLocation(EMonsterLeg Leg) const;

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

    void LegHasMovedEventCaller(EMonsterLeg MonsterLeg);


    // ------------------------------------------------------------------
    // Weapons
    // ------------------------------------------------------------------

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    float ArmourDamageReduction;

    UFUNCTION(BlueprintImplementableEvent)
    void MissileCollide(const FVector& HitLocation, const FVector& NormalizedDirection, const float DamageApplied);

    UFUNCTION(BlueprintCallable)
    void LooseWeapon(EMonsterWeaponType WeaponType);

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Weapons")
    TMap<EMonsterWeaponType, class AMonsterWeapon*> WeaponMap;

    // ------------------------------------------------------------------
    // AI
    // ------------------------------------------------------------------

    UPROPERTY(VisibleDefaultsOnly, Category = "AI")
    class UBehaviorTree* BehaviorTree;

    UPROPERTY(VisibleDefaultsOnly, Category = "AI")
    class AMonsterAIController* MonsterAI;

    // ------------------------------------------------------------------
    // DAMAGE & HEALTH
    // ------------------------------------------------------------------


    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health")
    float Health;

    /* Percentage value of the damage that is going to be applied */
    /* Should be from 0 to 1 */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health", meta = (UIMin = "0.0", UIMax = "1.0"))
    float DirectDamageReduction;

    UFUNCTION(BlueprintCallable, Category = "Health")
    void Die();

    UFUNCTION(BlueprintImplementableEvent, Category = "Health")
    void OnDieEvent();

    /* Handles damage from the weapon. Do not reduce the damage */
    UFUNCTION(BlueprintCallable, Category = "Health")
    void TakeNonDirectDamage(float Damage);

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
    bool bHandleDeathInCpp;

    /* Handles direct damage from the player. Reduces the actual damage  */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

protected:
    virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

    void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

    // ------------------------------------------------------------------
    // Legs
    // ------------------------------------------------------------------

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
    TArray<class UMonsterLegComponent*> Legs;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
    class UMonsterLegComponent* RearLeftLeg;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
    class UMonsterLegComponent* RearRightLeg;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
    class UMonsterLegComponent* FrontLeftLeg;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Legs")
    class UMonsterLegComponent* FrontRightLeg;

    UFUNCTION(BlueprintImplementableEvent)
    void LegHasMoved(EMonsterLeg Leg);

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
    // BODY MOVEMENT WHEN THERE IS AN OVERLAP FROM THE BODY TO THE LEGS
    // ------------------------------------------------------------------

    void ToggleWhatLegsShouldMove(bool Left) const;

    TArray<FName> TopSocketLocationNames;

    bool bBodyMoving;

    void CheckBodyAltitudeDependingOnLegs();


    // ------------------------------------------------------------------
    // Weapons
    // ------------------------------------------------------------------

    UPROPERTY( EditDefaultsOnly,BlueprintReadOnly, Category ="Weapons")
    FName FanSocketName;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category ="Weapons")
    FName SirenSocketName;

    UPROPERTY( EditDefaultsOnly,BlueprintReadOnly, Category ="Weapons")
    FName PipeSocketName;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category ="Weapons")
    FName TrafficLightSocketName;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category ="Weapons")
    FName PowerSystemSocketName;

    UFUNCTION(BlueprintCallable)
    void SpawnWeapons();


    UPROPERTY(EditDefaultsOnly, Category = "Weapons")
    TSubclassOf<class APipeMW> PipeClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapons")
    TSubclassOf<class AFanMW> FanClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapons")
    TSubclassOf<class ASirenMW> SirenClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapons")
    TSubclassOf<class ATrafficLightMW> TrafficLightClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapons")
    TSubclassOf<class APowerSystemMW> PowerSystemClass;
};

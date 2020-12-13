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

    UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent* MonsterMesh;

    UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USphereComponent* SphereComponent;

    UPROPERTY(Category = General, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UFloatingPawnMovement* PawnMovement;

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

    /* Weapons could be nulltptr when they got destroyed!!! Don't forget to check if its valid in BP */
    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class AFanMW* Fan;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class ASirenMW* Siren;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class ATrafficLightMW* TrafficLight;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class APipeMW* Pipe;

    UFUNCTION(BlueprintImplementableEvent)
    void MissileCollide(const FVector& HitLocation, const FVector& NormalizedDirection, const float DamageApplied);

    UFUNCTION(BlueprintCallable)
    void LooseWeapon(EMonsterWeaponType WeaponType);

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Weapons")
    TArray<class AMonsterWeapon*> Weapons;

    /* Use it to get a reference of the weapon if the task/ai controller. Could be nullptr because dissatached */
    FORCEINLINE UFUNCTION(Category = "Weapons")
    class AFanMW* GetFan(){ return Fan;}

    FORCEINLINE UFUNCTION(Category = "Weapons")
    class ASirenMW* GetSiren(){ return Siren;}

    FORCEINLINE UFUNCTION(Category = "Weapons")
    class APipeMW* GetPipe(){ return Pipe;}

    FORCEINLINE UFUNCTION(Category = "Weapons")
    class ATrafficLightMW* GetTrafficLight(){ return TrafficLight;}

    // ------------------------------------------------------------------
    // AI
    // ------------------------------------------------------------------

    UPROPERTY(EditAnywhere, Category = "AI")
    class UBehaviorTree* BehaviorTree;

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
};

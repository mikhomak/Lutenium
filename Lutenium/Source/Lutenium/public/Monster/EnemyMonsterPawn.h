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
    float Health;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    float ArmourDamageReduction;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class ASputnikMW* Sputnik;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class ASirenMW* Siren;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class ATrafficLightMW* TrafficLight;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category ="Weapons")
    class APipeMW* Pipe;

    UFUNCTION(BlueprintImplementableEvent)
    void MissileCollide(const FVector& HitLocation, const FVector& NormalizedDirection, const float DamageApplied);

    UFUNCTION(BlueprintCallable)
    void OnTakeDamage(float Damage);

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category ="Weapons")
    TArray<class AMonsterWeapon*> Weapons;

    /* Use it to get a reference of the weapon if the task/ai controller. Could be nullptr because dissatached */
    FORCEINLINE UFUNCTION(Category = "Weapons")
    class ASputnikMW* GetSputnik(){ return Sputnik;}

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


protected:
    virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

    void GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const override;

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
    FName SputnikSocketName;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category ="Weapons")
    FName SirenSocketName;

    UPROPERTY( EditDefaultsOnly,BlueprintReadOnly, Category ="Weapons")
    FName PipeSocketName;

    UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category ="Weapons")
    FName TrafficLightSocketName;
};

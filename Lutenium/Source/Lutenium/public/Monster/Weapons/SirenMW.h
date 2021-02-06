// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "SirenMW.generated.h"

/**
 * Siren weapon on top of the monster
 * Normal attack creates Scream that pushes or drags the player away
 * Upgraded version creates two screams
 * Also, when the players are near a SirenTriger, pushing them away
 */
UCLASS()
class LUTENIUM_API ASirenMW : public AMonsterWeapon
{
    GENERATED_BODY()
public:

    ASirenMW();

    virtual void ExecuteAttack() override;

    /** Before attack event */
    virtual void BeforeAttackEvent_Implementation() override;

    /**
     * Scream parent-class that will be shot from the top of the siren
     * Create BP child class from AScream and assign it to this variable
     */
    UPROPERTY(EditDefaultsOnly, Category = "Siren")
	TSubclassOf<class AScream> ScreamClass;

    /**
     * When the players are near a Siret Triger, determines the amount of pushing force applied to them
     */
    UPROPERTY(EditDefaultsOnly, Category="Siren trigger")
    float SirenForce;

    /**
     * Determins if the spawned Scream should drag the player of push away
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Siren")
    bool bDragOrImpulse;

    /**
     * Detemins which type of Screan should be spawned
     * true - normal scream
     * false - semisphere
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Siren")
    bool bIsScreamSemishpere;

    /**
     * On top of the siren
     * Pushing the players away when they are close
     */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* SirenTrigger;

    /**
     * Activates DragMomventEffect of the player
     */
    UFUNCTION(BlueprintCallable)
    void SirenTriggerOverlap(class AActor* OtherActor);

    /**
     * Deactivates DragMomventEffect of the player
     */
    UFUNCTION(BlueprintCallable)
    void SirenTriggerOverlapEnd(class AActor* OtherActor);
};

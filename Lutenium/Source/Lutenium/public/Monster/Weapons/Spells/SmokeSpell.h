#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmokeSpell.generated.h"

/**
 * Smoke that is released from the pipe
 * Slowly falling down, if overlapps with the player, adds post processing effect and slows the player down
 */
UCLASS()
class LUTENIUM_API ASMokeSpell : public AActor
{
	GENERATED_BODY()

public:

	ASMokeSpell();

    /**
     * Main projectile mesh
     * Root component
     */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Smoke")
    class UStaticMeshComponent* SmokeMesh;

    /* Smokes life time */
    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Smoke")
    float LifeTime;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


};

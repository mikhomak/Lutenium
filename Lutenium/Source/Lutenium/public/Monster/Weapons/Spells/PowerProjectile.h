#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerProjectile.generated.h"

/**
 * Projectiles that gets shoot from the fence tower
 */
UCLASS()
class LUTENIUM_API APowerProjectile : public AActor
{
	GENERATED_BODY()

public:

	APowerProjectile();

    /* Main projectile collider */
 	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Power Projectile")
    class USphereComponent* MainSphereComp;

    /* Main projectile mesh */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Power Projectile")
    class UStaticMeshComponent* ProjectileMesh;

    /* Projectile movement */
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Power Projectile")
    class UProjectileMovementComponent* ProjectileMovement;

	// ------------------------------------------------------------------
	// Stats
	// ------------------------------------------------------------------

    /* Push force on impact with the player */
    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Power Projectile")
    float PushForce;

    /* Life time of the projectile. Projectile gets destroyed on the end of the life spam */
    UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Power Projectile")
    float LifeTime;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


};



#include "Monster/Weapons/FenceTowerMW.h"
#include "Monster/Weapons/PowerSystemMW.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/Spells/PowerProjectile.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Player/PlayerPawn.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/MovementEffect/EmpMovementEffect.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Monster/Weapons/WeaponsUtils/FenceTower2DArray.h"
#include "AssistUtils/AssistUtils.h"
#include "Math/Vector.h"
#include "Components/ArrowComponent.h"


AFenceTowerMW::AFenceTowerMW() : AMonsterWeapon()
{
    /* Creating arrow projectile */
    ArrowProjectile = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Projectile"));
    ArrowProjectile ->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);

    Health = 100.f;
    BeamRadius = 2000.f;

    /* BEAM EMP */
    BeamEmpForce = 80.f;

    WeaponType = EMonsterWeaponType::FenceTower;
}


void AFenceTowerMW::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    /* Only the parent tower should raycast*/
    /* Raycasting for the player each tick when the beam is active */
    if(bParentFenceTower && bActiveBeam &&
       (PlayerEmpMovementEffect == nullptr || !PlayerEmpMovementEffect->Active))
    {
        FHitResult Hit;
        FVector RightNeighborLocation = RightNeighborFenceTower != nullptr ? RightNeighborFenceTower->GetActorLocation() : FVector::ZeroVector;
        FVector LeftNeighborLocation = LeftNeighborFenceTower != nullptr ? LeftNeighborFenceTower->GetActorLocation() : FVector::ZeroVector;

        /* Racyasting for both neighbors */
        APlayerPawn* Player = FAssistUtils::RaycastForPlayer(this, GetWorld(),
                                                             GetActorLocation(),
                                                             RightNeighborLocation, LeftNeighborLocation,
                                                             BeamRadius, Hit);
        if(Player && !bIsPlayerInBeam)
        {
            /* Setting emp movemnt effect */
            if(PlayerEmpMovementEffect == nullptr && Player &&
               Player->GetPlaneMovement() && Player->GetPlaneMovement()->EmpMovementEffect)
            {
                PlayerEmpMovementEffect = Player->GetPlaneMovement()->EmpMovementEffect;
            }

            const FVector RandomRotation = FVector(
			    FMath::RandRange(0.f, 1.f),
			    FMath::RandRange(0.f, 1.f),
			    FMath::RandRange(0.f, 1.f)
		    );
            /* Safe activating player's emp*/
            if(PlayerEmpMovementEffect)
            {
                PlayerEmpMovementEffect->Activate(RandomRotation, BeamEmpForce);
            }
            bIsPlayerInBeam = true; // Applying emp only on entering the beam
        }
        else if(bIsPlayerInBeam)
        {
            bIsPlayerInBeam = false;
        }
    }
}

/** Shoot projectiles */
void AFenceTowerMW::ExecuteAttack()
{
    if(PowerProjectileClass)
    {
        // just in case
        UWorld* World = GetWorld();
        if(World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            if(MonsterPawn)
            {
                SpawnParams.Instigator = MonsterPawn;
            }

            const FVector SpawnLocation = ArrowProjectile->GetComponentLocation();
            const FRotator SpawnRotation = ArrowProjectile->GetComponentRotation();
            World->SpawnActor<APowerProjectile>(PowerProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
        }
    }
}

void AFenceTowerMW::Die()
{
    if(bDebugDetach)
    {
        return;
    }

    /* Disabling defense beam in case it was active  */
    bActiveBeam = false;
    Destroy();

}


void AFenceTowerMW::BeforeAttackEvent_Implementation()
{
    /* OVERRIDE THIS EVENT IN BP */
}
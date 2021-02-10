

#include "Monster/Weapons/FenceTowerMW.h"
#include "Monster/Weapons/PowerSystemMW.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/Weapons/Spells/PowerProjectile.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Player/Missile.h"
#include "Player/PlayerPawn.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/MovementEffect/EmpMovementEffect.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Monster/Weapons/WeaponsUtils/FenceTower2DArray.h"
#include "AssistUtils/AssistUtils.h"
#include "DrawDebugHelpers.h"
#include "Math/Vector.h"
#include "Components/ArrowComponent.h"


AFenceTowerMW::AFenceTowerMW() : AMonsterWeapon()
{
    /* Creating arrow projectile */
    const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
        EAttachmentRule::KeepRelative, false);
    ArrowProjectile = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Projectile"));
    ArrowProjectile ->AttachToComponent(WeaponMesh, AttachmentTransformRules);

    Health = 100.f;
    BeamRadius = 2000.f;

    /* BEAM EMP */
    BeamEmpForce = 80.f;

    WeaponType = EMonsterWeaponType::FenceTower;
    bIsActiveTower = true;
}


void AFenceTowerMW::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    /* Only the parent tower should raycast*/
    /* Raycasting for the player each tick when the beam is active */
    if(bParentFenceTower && bActiveBeam &&
       (PlayerEmpMovementEffect == nullptr || !PlayerEmpMovementEffect->Active))
    {
        for(auto NeighborTower : BeamFenceTowers)
        {
            if(NeighborTower)
            {
                if(NeighborTower->bIsActiveTower == false)
                {
                    return;
                }
                FHitResult Hit;
                FVector NeighborLocation = NeighborTower != nullptr ? NeighborTower->ArrowProjectile->GetComponentLocation() : FVector::ZeroVector;

                /* Racyasting for both neighbors towers, finding for the player or the missile  */
                AActor* FoundActor = FAssistUtils::RaycastForPlayer(this, GetWorld(),
                                                                  ArrowProjectile->GetComponentLocation(),
                                                                  NeighborLocation,
                                                                  BeamRadius, Hit);

                if(FoundActor == nullptr)
                {
                    bIsPlayerInBeam = false;
                    continue;
                }


                /* Handles player */
                APlayerPawn* Player = Cast<APlayerPawn>(FoundActor);
                if(Player)
                {
                    PlayerHasEnteredTheBeamEven(FoundActor);
                    HandlePlayerBeam(Player);
                    return;
                }

                bIsPlayerInBeam = false;

                /* Handles missile */
                AMissile* Missile = Cast<AMissile>(FoundActor);
                if(Missile)
                {
                    HandleMissileBeam(Missile);
                    return;
                }
            }
        }
    }
}


void AFenceTowerMW::HandlePlayerBeam(APlayerPawn* Player)
{
    if(!bIsPlayerInBeam)
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
        return;
    }
}

void AFenceTowerMW::HandleMissileBeam(AMissile* Missile)
{
    Missile->EmpMissile();
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
    /* Disabling defense beam in case it was active  */
    bActiveBeam = false;
    bIsActiveTower = false;


    if(bDebugDetach)
    {
        return;
    }


    Destroy();

}


void AFenceTowerMW::BeforeAttackEvent_Implementation()
{
    /* OVERRIDE THIS EVENT IN BP */
}
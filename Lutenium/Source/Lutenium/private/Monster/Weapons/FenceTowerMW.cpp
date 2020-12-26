

#include "Monster/Weapons/FenceTowerMW.h"
#include "Monster/Weapons/PowerSystemMW.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Player/PlayerPawn.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/MovementEffect/EmpMovementEffect.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Monster/Weapons/WeaponsUtils/FenceTower2DArray.h"
#include "AssistUtils/AssistUtils.h"
#include "Math/Vector.h"


AFenceTowerMW::AFenceTowerMW() : AMonsterWeapon()
{
    Health = 100.f;
    BeamRadius = 2000.f;

    /* BEAM EMP */
    BeamEmpTime = 5.5f;
    BeamEmpForce = 80.f;

    WeaponType = EMonsterWeaponType::FenceTower;
}


void AFenceTowerMW::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    /* Only the parent tower should raycast*/
    /* Raycasting for the player each tick when the beam is active */
    if(bParentFenceTower && bActiveBeam &&
       LeftNeighborFenceTower && RightNeighborFenceTower &&
       (PlayerEmpMovementEffect == nullptr || !PlayerEmpMovementEffect->Active))
    {
        FHitResult Hit;
        FVector RightNeighborLocation = RightNeighborFenceTower->GetActorLocation();
        FVector LeftNeighborLocation = LeftNeighborFenceTower->GetActorLocation();

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
                PlayerEmpMovementEffect = PlayerPawn->GetPlaneMovement()->EmpMovementEffect;
            }

            const FVector RandomRotation = FVector(
			    FMath::RandRange(0.f, 1.f),
			    FMath::RandRange(0.f, 1.f),
			    FMath::RandRange(0.f, 1.f)
		    );
            /* Safe activating player's emp*/
            if(PlayerEmpMovementEffect)
            {
                PlayerEmpMovementEffect->Activate(BeamEmpTime, RandomRotation, BeamEmpForce);
            }
            bIsPlayerInBeam = true; // Applying emp only on entering the beam
        }
        else if(bIsPlayerInBeam)
        {
            bIsPlayerInBeam = false;
        }
    }
}

void AFenceTowerMW::ExecuteAttack()
{

}

void AFenceTowerMW::Die()
{
    if(bDebugDetach)
    {
        return;
    }

    /* Disabling defense beam in case it was active  */
    bActiveBeam = false;

    /* Removes tower fense from power system */
    FFenceTower2DArray& FenceTowers = PowerSystem->FenceTowers[HightIndex];
    if(FenceTowers.Num() != 0)
    {
        if(FenceTowers.IsValidIndex(PositionIndex))
        {
            /* Removes the tower from the array without shrinking it!! */
            /* We don't shrink it so we can acces other towers directly by their position index */
            FenceTowers.RemoveAt(PositionIndex, 1, false);
        }
    }

}


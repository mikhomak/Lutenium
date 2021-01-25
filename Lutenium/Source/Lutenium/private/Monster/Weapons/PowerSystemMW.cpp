

#include "Monster/Weapons/PowerSystemMW.h"
#include "Monster/Weapons/FenceTowerMW.h"
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Monster/Weapons/WeaponsUtils/FenceTower2DArray.h"


APowerSystemMW::APowerSystemMW() : AMonsterWeapon()
{
    Health = 100.f;
    WeaponType = EMonsterWeaponType::PowerSystem;

    CooldownTime = 20.f;
    RepeatAttackTime = 5.f;

    /* Fence tower array initalization variables */
    FenceTowersHightLevelsAmount = 3;
    FenceTowersPositionsAmount = 4;
    TowerFenceSocketFormat = "FenceTowerHight{0}Position{1}Socket";
}

void APowerSystemMW::InitializeTowerFencse()
{
    /* Initialize Fence Towers */

    /* Spawn Params */
    FActorSpawnParameters SpawnParams;

    SpawnParams.Owner = MonsterPawn;
    SpawnParams.Instigator = MonsterPawn;

    if(FenceTowerClass)
    {
        for(int32 HightIndex = 0; HightIndex < FenceTowersHightLevelsAmount; HightIndex++)
        {
            FenceTowers.Add(FFenceTower2DArray());
            for(int32 PositionIndex = 0; PositionIndex < FenceTowersPositionsAmount; PositionIndex++)
            {
                AFenceTowerMW* SpawnedFenceTower = GetWorld()->SpawnActor<AFenceTowerMW>(FenceTowerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
                if(SpawnedFenceTower)
                {
                    SpawnedFenceTower->AttachToComponent(MonsterMesh,
                                                        FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false),
                                                        ConstructSocketName(HightIndex, PositionIndex)); // Dynmaicly creates socket name
                    /* Setting monster params for the spawned towers*/
                    SpawnedFenceTower->MonsterPawn = MonsterPawn;
                    SpawnedFenceTower->MonsterMesh = MonsterMesh;
                    SpawnedFenceTower->PowerSystem = this;
                    /* Setting indexes for spawned tower so even if we remove one of them from the array, we still would know the original index to adapt behaviour */
                    SpawnedFenceTower->HightIndex = HightIndex;
                    SpawnedFenceTower->PositionIndex = PositionIndex;
                    /* Setting parent for 0 and 1 towers */
                    if(PositionIndex == 0 || PositionIndex == 3)
                    {
                        SpawnedFenceTower->bParentFenceTower = true;
                    }
                    /* Adds spawned tower to the array of the current hight */
                    FenceTowers[HightIndex].Add(SpawnedFenceTower);
                }
            }
            /* Setting the neighbors for the parent towers */
            /* Setting them after we initialize all the towers for the currnet hight index*/
            if(FenceTowers[HightIndex][1] && FenceTowers[HightIndex][2])
            {
                if(FenceTowers[HightIndex][0])
                {
                    FenceTowers[HightIndex][0]->LeftNeighborFenceTower = FenceTowers[HightIndex][1];
                    FenceTowers[HightIndex][0]->RightNeighborFenceTower = FenceTowers[HightIndex][2];
                }
                if(FenceTowers[HightIndex][3])
                {
                    FenceTowers[HightIndex][3]->LeftNeighborFenceTower = FenceTowers[HightIndex][1];
                    FenceTowers[HightIndex][3]->RightNeighborFenceTower = FenceTowers[HightIndex][2];
                }
            }

        }
    }
}

FName APowerSystemMW::ConstructSocketName(int32 HightIndex, int32 PositionIndex)
{
    return FName(FString::Format(*TowerFenceSocketFormat, { HightIndex, PositionIndex }));
}

/** shoots from every tower */
void APowerSystemMW::ExecuteAttack()
{
    for(int32 HightIndex = 0; HightIndex < FenceTowers.Num(); HightIndex++)
    {
        for(int32 PositionIndex = 0; PositionIndex < FenceTowers[HightIndex].Num(); PositionIndex++)
        {
            if(FenceTowers[HightIndex][PositionIndex])
            {
                FenceTowers[HightIndex][PositionIndex]->ExecuteAttack();
            }
        }
    }

    /* When the upgrade is >= 1 and Siren spawned less than 2 sirens, creates a timer for another one */
    if(LevelUpgrade >= 1 && CurrentExecutedAttacks < 2)
    {
        FTimerHandle SecondAttackTimer;
        GetWorldTimerManager().SetTimer(SecondAttackTimer, this, &APowerSystemMW::ExecuteAttack, RepeatAttackTime, false);
        CurrentExecutedAttacks++;
    }
}


void APowerSystemMW::ActivateBeamDefense()
{
    /* Activates beam defense for every fence tower */
    for(int32 HightIndex = 0; HightIndex < FenceTowers.Num(); HightIndex++)
    {
        if(FenceTowers.IsValidIndex(HightIndex))
        {
            // Numbers are the positions of tower fences on the legs. Monster is the head. Dashes are beams.
            //
            //
            //    0---------------------1
            //    |      MONSTER
            //    |
            //    2                     3
            SafeActiveBeam(true, FenceTowers[HightIndex][0], true);
            SafeActiveBeam(true, FenceTowers[HightIndex][0], false);

            //    0                     1
            //           MONSTER        |
            //                          |
            //    2---------------------3
            SafeActiveBeam(true, FenceTowers[HightIndex][3], true);
            SafeActiveBeam(true, FenceTowers[HightIndex][3], false);
        }
    }
}


void APowerSystemMW::DeactivateBeamDefense()
{
    /* Deactivate beam defense for every fence tower */
    for(int32 HightIndex = 0; HightIndex < FenceTowers.Num(); HightIndex++)
    {
        SafeActiveBeam(false, FenceTowers[HightIndex][0], true);
        SafeActiveBeam(false, FenceTowers[HightIndex][0], false);
        SafeActiveBeam(false, FenceTowers[HightIndex][3], true);
        SafeActiveBeam(false, FenceTowers[HightIndex][3], false);
    }
}

void APowerSystemMW::SafeActiveBeam(bool bActivate, class AFenceTowerMW* FenceTowerStart, bool bLeft)
{
    if(FenceTowerStart)
    {
        FenceTowerStart->SetActiveBeam(bActivate, bLeft);
    }
}

void APowerSystemMW::BeforeAttackEvent_Implementation()
{
    /* OVERRIDE THIS EVENT IN BP */
}

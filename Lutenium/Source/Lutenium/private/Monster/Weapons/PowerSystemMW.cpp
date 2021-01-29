

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
    FenceTowersPositionsAmount = 6;
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
                    /* Setting parent for 1, 2 and 5 towers */
                    //
                    //
                    //      LEFT    0---------------------1  RIGHT
                    //              |                     |
                    //              |                     |
                    //              2       MONSTER       3
                    //              |                     |
                    //              |                     |
                    //              4---------------------5
                    //
                    //
                    /* Parent towers are only 1, 2 and 5, because by using only those parents we can cover every beam */
                    /* 2 create a beam to 0 and 4 */
                    /* 1 to 0 and 3 */
                    /* 5 to 4 and 3*/
                    if(PositionIndex == 1 || PositionIndex == 2 || PositionIndex == 5 )
                    {
                        SpawnedFenceTower->bParentFenceTower = true;
                    }
                    /* Adds spawned tower to the array of the current hight */
                    FenceTowers[HightIndex].Insert(SpawnedFenceTower, PositionIndex);
                }
            }
            /* Setting the neighbors for the parent towers */
            /* Setting them after we initialize all the towers for the currnet hight index*/
            /* 2 create a beam to 0 and 4 */
            /* 1 to 0 and 3 */
            /* 5 to 4 and 3*/
            if(FenceTowers[HightIndex][0] && FenceTowers[HightIndex][3] && FenceTowers[HightIndex][4])
            {
                if(FenceTowers[HightIndex][1])
                {
                    FenceTowers[HightIndex][1]->BeamFenceTowers.Add(FenceTowers[HightIndex][0]);
                    FenceTowers[HightIndex][1]->BeamFenceTowers.Add(FenceTowers[HightIndex][3]);
                }
                if(FenceTowers[HightIndex][5])
                {
                    FenceTowers[HightIndex][5]->BeamFenceTowers.Add(FenceTowers[HightIndex][3]);
                    FenceTowers[HightIndex][5]->BeamFenceTowers.Add(FenceTowers[HightIndex][4]);
                }
                if(FenceTowers[HightIndex][2])
                {
                    FenceTowers[HightIndex][2]->BeamFenceTowers.Add(FenceTowers[HightIndex][0]);
                    FenceTowers[HightIndex][2]->BeamFenceTowers.Add(FenceTowers[HightIndex][4]);
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
            //      LEFT    0                     1  RIGHT
            //              |
            //              |
            //              2       MONSTER       3
            //              |
            //              |
            //              4                     5
            //
            //
            SafeActiveBeam(true, FenceTowers[HightIndex][2], true);
            SafeActiveBeam(true, FenceTowers[HightIndex][2], false);

            //
            //
            //      LEFT    0---------------------1  RIGHT
            //                                    |
            //                                    |
            //              2       MONSTER       3
            //
            //
            //              4                     5
            //
            //
            SafeActiveBeam(true, FenceTowers[HightIndex][1], true);
            SafeActiveBeam(true, FenceTowers[HightIndex][1], false);

            //
            //
            //      LEFT    0                     1  RIGHT
            //
            //
            //              2       MONSTER       3
            //                                    |
            //                                    |
            //              4---------------------5
            //
            //
            SafeActiveBeam(true, FenceTowers[HightIndex][5], true);
            SafeActiveBeam(true, FenceTowers[HightIndex][5], false);

        }
    }
}


void APowerSystemMW::DeactivateBeamDefense()
{
    /* Deactivate beam defense for every fence tower */
    for(int32 HightIndex = 0; HightIndex < FenceTowers.Num(); HightIndex++)
    {
        SafeActiveBeam(false, FenceTowers[HightIndex][2], true);
        SafeActiveBeam(false, FenceTowers[HightIndex][2], false);
        SafeActiveBeam(false, FenceTowers[HightIndex][1], true);
        SafeActiveBeam(false, FenceTowers[HightIndex][1], false);
        SafeActiveBeam(false, FenceTowers[HightIndex][5], true);
        SafeActiveBeam(false, FenceTowers[HightIndex][5], false);
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


void APowerSystemMW::SpecificUpgrade(int32 Level)
{
    int NeighborHights[2]; /* Creates an arrya of existed hight neighbors. It can be lower hight, higher or both */
    for(int32 HightIndex = 0; HightIndex < FenceTowers.Num(); HightIndex++)
    {
        NeighborHights[0] = FenceTowers.IsValidIndex(HightIndex + 1) ? HightIndex + 1 : -1;
        NeighborHights[1] = FenceTowers.IsValidIndex(HightIndex - 1) ? HightIndex - 1 : -1;

        for(auto NeighborHightIndex : NeighborHights)
        {
            if(NeighborHightIndex != -1) /* add new neighbors only if the index is valid */
            {

                //
                //
                //      LEFT    0---------------------1  RIGHT
                //                                    |
                //                                    |
                //              2       MONSTER       3
                //
                //
                //              4                     5
                //
                //
                SaveAddNewNeighborsToTheParentTower(HightIndex, 1, NeighborHightIndex, 0, 3);

                // Numbers are the positions of tower fences on the legs. Monster is the head. Dashes are beams.
                //
                //
                //      LEFT    0                     1  RIGHT
                //              |
                //              |
                //              2       MONSTER       3
                //              |
                //              |
                //              4                     5
                //
                //
                SaveAddNewNeighborsToTheParentTower(HightIndex, 2, NeighborHightIndex, 0, 4);

                //
                //      LEFT    0                     1  RIGHT
                //
                //
                //              2       MONSTER       3
                //                                    |
                //                                    |
                //              4---------------------5
                //
                //
                SaveAddNewNeighborsToTheParentTower(HightIndex, 5, NeighborHightIndex, 3, 4);
            }
        }
    }
}

void APowerSystemMW::SaveAddNewNeighborsToTheParentTower(int ParentHightIndex,
                                         int ParentPositionIndex,
                                         int NeighborsHightIndex,
                                         int LeftNeighborPositionIndex,
                                         int RightNeighborPositionIndex)
{
    if(FenceTowers[ParentHightIndex].IsValidIndex(ParentPositionIndex))
    {
        if(FenceTowers[NeighborsHightIndex].IsValidIndex(LeftNeighborPositionIndex))
        {
            FenceTowers[ParentHightIndex][ParentPositionIndex]->BeamFenceTowers.Add(FenceTowers[NeighborsHightIndex][LeftNeighborPositionIndex]);
        }
        if(FenceTowers[NeighborsHightIndex].IsValidIndex(RightNeighborPositionIndex))
        {
            FenceTowers[ParentHightIndex][ParentPositionIndex]->BeamFenceTowers.Add(FenceTowers[NeighborsHightIndex][RightNeighborPositionIndex]);
        }
    }
}
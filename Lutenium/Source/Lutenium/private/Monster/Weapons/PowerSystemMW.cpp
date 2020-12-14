

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

    /* Fence tower array initalization variables */
    FenceTowersHightLevelsAmount = 3;
    FenceTowersPositionsAmount = 4;
    TowerFenceSocketFormat = "FenceTowerHight{}Position{}Socket";
}

void APowerSystemMW::BeginPlay()
{
    /* Initialize Fence Towers */

    /* Spawn Params */
    FActorSpawnParameters SpawnParams;

    SpawnParams.Owner = MonsterPawn;
    SpawnParams.Instigator = MonsterPawn;


    for(int32 HightIndex = 0; HightIndex < FenceTowersHightLevelsAmount; HightIndex++)
    {
        FenceTowers.Add(FFenceTower2DArray());
        for(int32 PositionIndex = 0; HightIndex < FenceTowersPositionsAmount; PositionIndex++)
        {
            AFenceTowerMW* SpawnedFenceTower = GetWorld()->SpawnActor<AFenceTowerMW>(FenceTowerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
            if(SpawnedFenceTower)
            {
                SpawnedFenceTower->AttachToComponent(MonsterMesh,
                                                    FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, false),
                                                    ConstructSocketName(HightIndex, PositionIndex)); // Dynmaicly creates socket name
                /* Setting monster params for the spawned towers*/
                SpawnedFenceTower->MonsterPawn = MonsterPawn;
                SpawnedFenceTower->MonsterMesh = MonsterMesh;
                SpawnedFenceTower->PowerSystem = this;
                /* Setting indexes for spawned tower so even if we remove one of them from the array, we still would know the original index to adapt behaviour */
                SpawnedFenceTower->HightIndex = HightIndex;
                SpawnedFenceTower->PositionIndex = PositionIndex;
                /* Adds spawned tower to the array of the current hight */
                FenceTowers[HightIndex].Add(SpawnedFenceTower);
            }
        }
    }
}

FName APowerSystemMW::ConstructSocketName(int32 HightIndex, int32 PositionIndex)
{
    return FName(FString::Format(*TowerFenceSocketFormat, { HightIndex, PositionIndex }));
}

void APowerSystemMW::ExecuteAttack()
{

}


void APowerSystemMW::ActivateBeamDefense()
{
    /* Activates beam defense for every fence tower */
    for(int32 HightIndex = 0; HightIndex < FenceTowers.Num(); HightIndex++)
    {
        // Numbers are the positions of tower fences on the legs. Monster is the head. Dashes are beams.
        //
        //
        //    0---------------------1
        //    |      MONSTER
        //    |
        //    2                     3
        SafeActiveBeam(true, FenceTowers[HightIndex][0], FenceTowers[HightIndex][1], FenceTowers[HightIndex][2]);


        //    0                     1
        //           MONSTER        |
        //                          |
        //    2---------------------3
        SafeActiveBeam(true, FenceTowers[HightIndex][3], FenceTowers[HightIndex][1], FenceTowers[HightIndex][2]);
    }
}


void APowerSystemMW::DeactivateBeamDefense()
{
    /* Deactivate beam defense for every fence tower */
    for(int32 HightIndex = 0; HightIndex < FenceTowers.Num(); HightIndex++)
    {
        SafeActiveBeam(false, FenceTowers[HightIndex][1], FenceTowers[HightIndex][0], FenceTowers[HightIndex][2]);
        SafeActiveBeam(false, FenceTowers[HightIndex][3], FenceTowers[HightIndex][0], FenceTowers[HightIndex][2]);
    }
}

void APowerSystemMW::SafeActiveBeam(bool bActivate, class AFenceTowerMW* FenceTowerStart, class AFenceTowerMW* FirstTargetFenceTower, class AFenceTowerMW* SecondTargetFenceTower)
{
    if(FenceTowerStart)
    {
        if(FirstTargetFenceTower)
        {
            FenceTowerStart->SetActiveBeam(true, FirstTargetFenceTower->GetActorLocation());
        }
        if(SecondTargetFenceTower)
        {
            FenceTowerStart->SetActiveBeam(true, SecondTargetFenceTower->GetActorLocation());
        }
    }
}


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
    TowerFenceSocketFormat = "FenceTowerHight{}Position{}";
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
        for(int32 PositionIndex = 0; PositionIndex < FenceTowers[HightIndex].Num(); PositionIndex++)
        {
            FenceTowers[HightIndex][PositionIndex]->SetActiveBeam(true);
        }
    }
}


void APowerSystemMW::DeactivateBeamDefense()
{
    /* Deactivate beam defense for every fence tower */
    for(int32 HightIndex = 0; HightIndex < FenceTowers.Num(); HightIndex++)
    {
        for(int32 PositionIndex = 0; PositionIndex < FenceTowers[HightIndex].Num(); PositionIndex++)
        {
            FenceTowers[HightIndex][PositionIndex]->SetActiveBeam(false);
        }
    }
}


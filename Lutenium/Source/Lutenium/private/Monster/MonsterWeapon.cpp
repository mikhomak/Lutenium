#include "../../public/Monster/MonsterWeapon.h"
#include "../../public/Monster/EnemyMonsterPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../public/Monster/Weapons/Scream.h"

UMonsterWeapon::UMonsterWeapon()
{
    PrimaryComponentTick.bCanEverTick = true;
}


void UMonsterWeapon::DoSiren() const
{
    UWorld* World = GetWorld();
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = MonsterPawn;
        SpawnParams.Instigator = MonsterPawn;

        FVector SpawnLocation = MonsterMesh->GetSocketLocation(SirenSocketName);

        AScream* Scream = World->SpawnActor<AScream>(SirenClass, SpawnLocation, FRotator::ZeroRotator,
                                                      SpawnParams);
        if (Scream)
        {

        }
    }

}

void UMonsterWeapon::BeginPlay()
{
    Super::BeginPlay();
}

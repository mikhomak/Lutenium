#include "Monster/Weapons/SirenMW.h"
#include "Monster/Weapons/Spells/Scream.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Player/PlayerPawn.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/MovementEffect/DragMovementEffect.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"



ASirenMW::ASirenMW() : AMonsterWeapon()
{
    Health = 100.f;
    SirenTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Siren trigger"));
    SirenTrigger->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);

    WeaponType = EMonsterWeaponType::Siren;

    /* Set events in BP for overlapping!*/
}

void ASirenMW::ExecuteAttack()
{
    if (ScreamClass)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = MonsterPawn;
            SpawnParams.Instigator = MonsterPawn;
            AScream* Scream = World->SpawnActor<AScream>(ScreamClass, SirenTrigger->GetComponentTransform().GetLocation(), GetActorRotation(),
                                                            SpawnParams);
            if(Scream)
            {
                Scream->SetDragOrImpulse(bDragOrImpulse);
            }
        }
    }
}

void ASirenMW::SirenTriggerOverlap(class AActor* OtherActor)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
        FVector DragDirection = PlayerPawn->GetActorLocation() - SirenTrigger->GetComponentTransform().GetLocation();
        DragDirection.Normalize();
        PlayerPawn->GetPlaneMovement()->DragMovementEffect->Activate(SirenForce, DragDirection);
    }
}

void ASirenMW::SirenTriggerOverlapEnd(class AActor* OtherActor)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
        PlayerPawn->GetPlaneMovement()->DragMovementEffect->Deactivate();
    }
}

void ASirenMW::BeforeAttackEvent_Implementation()
{
    
}
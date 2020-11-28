// Fill out your copyright notice in the Description page of Project Settings.


#include "../../../public/Monster/Weapons/SirenMW.h"
#include "../../../public/Monster/Weapons/Scream.h"
#include "../../../public/Monster/EnemyMonsterPawn.h"
#include "../../../public/Player/PlayerPawn.h"
#include "../../../public/Player/PlaneMovementComponent.h"
#include "../../../public/Player/MovementEffect/DragMovementEffect.h"
#include "../../../public/Monster/Weapons/MonsterWeaponType.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"



ASirenMW::ASirenMW() : AMonsterWeapon()
{
    Health = 100.f;
    SirenTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Siren trigger"));
    SirenTrigger->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);

    //WeaponType = EMonsterWeaponType::Siren;

    /* Set events in BP for overlapping!*/
}

void ASirenMW::DoSiren(bool bDragOrImpulse)
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
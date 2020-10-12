// Fill out your copyright notice in the Description page of Project Settings.


#include "../../../public/Monster/Weapons/SirenMW.h"
#include "../../../public/Player/PlayerPawn.h"
#include "../../../public/Player/PlaneMovementComponent.h"
#include "../../../public/Player/MovementEffect/DragMovementEffect.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"


ASirenMW::ASirenMW() : AMonsterWeapon()
{
    Health = 100.f;
    SirenTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Siren trigger"));
    /* Set events in BP for overlapping!*/
}

void ASirenMW::Do_Siren()
{

}

void ASirenMW::SirenTriggerOverlap(
    class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
        FVector DragDirection = PlayerPawn->GetActorLocation() - GetActorLocation();
        DragDirection.Normalize();
        PlayerPawn->GetPlaneComponent()->DragMovementEffect->Activate(SirenForce, DragDirection);
    }
}

void ASirenMW::SirenTriggerOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                            class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
        PlayerPawn->GetPlaneComponent()->DragMovementEffect->Deactivate();
    }
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "../../../public/Monster/Weapons/TrafficLightMW.h"
#include "../../../public/Monster/Weapons/TrafficLight.h"
#include "../../../public/Monster/Weapons/TrafficLightPosition.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

ATrafficLightMW::ATrafficLightMW() : AMonsterWeapon()
{
    Health = 100.f;


    /* Creating Lights meshes */
    LeftLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Light Mesh"));
    RightLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Light Mesh"));
    CenterLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Center Light Mesh"));

    LeftLightMesh->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);
    RightLightMesh->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);
    CenterLightMesh->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);
}

void ATrafficLightMW::ChangeLight(ETrafficLightPosition Position, ETrafficLight Light)
{
    OnLightChanged(Position, Light);
}


void ATrafficLightMW::RightTriggerOverlap(
    class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor,
    class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult &SweepResult)
{
}

void ATrafficLightMW::RightTriggerOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor,
                                             class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
}

void ATrafficLightMW::LeftTriggerOverlap(
    class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor,
    class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult &SweepResult)
{
}

void ATrafficLightMW::LeftTriggerOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor,
                                            class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
}

void ATrafficLightMW::CenterTriggerOverlap(
    class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor,
    class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult &SweepResult)
{
}

void ATrafficLightMW::CenterTriggerOverlapEnd(class UPrimitiveComponent *OverlappedComp, class AActor *OtherActor,
                                              class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex)
{
}
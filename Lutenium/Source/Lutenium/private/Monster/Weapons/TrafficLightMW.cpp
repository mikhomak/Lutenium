// Fill out your copyright notice in the Description page of Project Settings.

#include "../../../public/Monster/Weapons/TrafficLightMW.h"
#include "../../../public/Monster/Weapons/TrafficLight.h"
#include "../../../public/Monster/Weapons/TrafficLightPosition.h"
#include "../../../public/Monster/Weapons/MonsterWeaponType.h"
#include "../../../public/Player/Missile.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

ATrafficLightMW::ATrafficLightMW() : AMonsterWeapon()
{
    Health = 100.f;


    WeaponType = EMonsterWeaponType::TrafficLight;

    /* Creating Lights meshes */
    LeftLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Light Mesh"));
    RightLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Light Mesh"));
    CenterLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Center Light Mesh"));

    LeftLightMesh->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);
    RightLightMesh->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);
    CenterLightMesh->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);

    MissileThrowForce = 6000.f;
}

void ATrafficLightMW::ChangeLight(ETrafficLightPosition Position, ETrafficLight Light)
{
    OnLightChanged(Position, Light);
}


void ATrafficLightMW::LightBeginOverlap(class AActor* Actor, const ETrafficLight TrafficLightStatus, const ETrafficLightPosition TrafficLightPosition)
{
    FVector Position;
    switch(TrafficLightPosition){
        case ETrafficLightPosition::Right:
            Position = RightLightMesh->GetComponentLocation();
        break;
        case ETrafficLightPosition::Left:
            Position = LeftLightMesh->GetComponentLocation();
        break;
        case ETrafficLightPosition::Center:
            Position = CenterLightMesh->GetComponentLocation();
        break;
        default:
            Position = GetActorLocation();
    }
    AMissile* Missile = Cast<AMissile>(Actor);
    if(Missile)
    {
        Position = Missile->GetActorLocation() - Position;
        Position.Normalize();
        Missile->ThrowMissile(Missile->GetActorForwardVector() * -1, MissileThrowForce);
    }
}
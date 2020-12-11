#include "Monster/Weapons/TrafficLightMW.h"
#include "Monster/Weapons/TrafficLight.h"
#include "Monster/Weapons/TrafficLightPosition.h"
#include "Monster/Weapons/MonsterWeaponType.h"
#include "Player/Missile.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

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
    switch(TrafficLightPosition)
    {
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


    /* Handles the missile overlap */
    /* Missile gets thrown away if the light is not green, otherwise do nothing */
    AMissile* Missile = Cast<AMissile>(Actor);
    if(TrafficLightStatus != ETrafficLight::Green && Missile)
    {
        /* If the light is red, the missile should become defected */
        const bool bDefected = TrafficLightStatus == ETrafficLight::Red;

        /* Creates event to handle some VFX, SFX */
        OnMissileGetThrownAway(TrafficLightPosition, TrafficLightStatus, Missile->GetActorLocation(), bDefected);

        /* Throws missile in the opposite direction */
        Position = Missile->GetActorLocation() - Position;
        Position.Normalize();
        Missile->ThrowMissile(Missile->GetActorForwardVector() * -1, MissileThrowForce);
        if(bDefected)
        {
            Missile->ActivateDefected();
        }
    }
}

void ATrafficLightMW::ExecuteAttack(){}
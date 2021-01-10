#include "Monster/Weapons/TrafficLightMW.h"
#include "Monster/Weapons/WeaponsUtils/TrafficLight.h"
#include "Monster/Weapons/WeaponsUtils/TrafficLightPosition.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Player/Missile.h"
#include "Player/PlayerPawn.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/MovementEffect/DragMovementEffect.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

#define ECC_MonsterWPHurtbox ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Monster ECollisionChannel::ECC_GameTraceChannel2
#define ECC_MonsterSpell ECollisionChannel::ECC_GameTraceChannel3

ATrafficLightMW::ATrafficLightMW() : AMonsterWeapon()
{
    /* Creates two more hurtboxes*/
    HurtboxRight = CreateDefaultSubobject<USphereComponent>(TEXT("Hurt box right"));
    HurtboxRight->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);
    HurtboxRight->SetCollisionProfileName(TEXT("MonsterWPHurtbox"));

    HurtboxLeft = CreateDefaultSubobject<USphereComponent>(TEXT("Hurt box left"));
    HurtboxLeft->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);
    HurtboxLeft->SetCollisionProfileName(TEXT("MonsterWPHurtbox"));

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
    PlayerDragForce = 7000.f;

}

void ATrafficLightMW::BeginPlay()
{
    /* Setting up default lights on the begging */
    ChangeLight(ETrafficLightPosition::Right, ETrafficLight::Green);
    ChangeLight(ETrafficLightPosition::Center, ETrafficLight::Red);
    ChangeLight(ETrafficLightPosition::Left, ETrafficLight::Yellow);
}

/** Changes the light of the mesh light (duh) */
/** Red - throws defected missile away */
/** Yellow - throws normal missile away */
/** Green - allows missile to hit the target */
void ATrafficLightMW::ChangeLight(ETrafficLightPosition Position, ETrafficLight Light)
{
    switch(Position)
    {
        case ETrafficLightPosition::Right:
            CenterLightStatus = Light;
        break;
        case ETrafficLightPosition::Left:
            LeftLightStatus = Light;
        break;
        case ETrafficLightPosition::Center:
            RightLightStatus = Light;
        break;
    }
    /* Setting the latest red light position so we could use it for the attack */
    SetLatestRedLightPosition(Light, Position);
    /* Change the material of the Light mesh in BP */
    OnLightChanged(Position, Light);
}

/** Handles overlaps of the lights */
/** Handles missile and player */
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
        return;
    }

    /* Handles player overlap */
    /* If the player overlaps the red light, drags him away from the light */
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(Actor);
    if (TrafficLightStatus == ETrafficLight::Red && PlayerPawn)
    {
        FVector DragDirection = PlayerPawn->GetActorLocation() - Position;
        DragDirection.Normalize();
        PlayerPawn->GetPlaneMovement()->DragMovementEffect->Activate(PlayerDragForce, DragDirection);
        return;
    }
}

/* Scales the current red light if facing the player */
void ATrafficLightMW::ExecuteAttack()
{

}

/** Handles events on end overlap  */
/** If it's a player then we should deactivate drag (probably it was activated by overlapping with the red light) */
void ATrafficLightMW::LightEndOverlap(class AActor* Actor, const ETrafficLight TrafficLightStatus, const ETrafficLightPosition TrafficLightPosition)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(Actor);
    if(PlayerPawn)
    {
        PlayerPawn->GetPlaneMovement()->DragMovementEffect->Deactivate();
    }
}


float ATrafficLightMW::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{

    if(!DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
    {
        return 0.f;
    }
    const auto RadialDamage = (FRadialDamageEvent*)&DamageEvent;
    /* getting collision object type to difirentatite mesh from other parts of the weapon */
    const ECollisionChannel ComponentCollisionChannel = RadialDamage->ComponentHits[0].Component.Get()->GetCollisionObjectType();

    if(ComponentCollisionChannel == ECC_Monster)
    {
        TakeMeshDamage(Damage);
        return Damage;
    }
    /* If it wasn't mesh, then it would be hurtbox*/
    if(ComponentCollisionChannel == ECC_MonsterWPHurtbox || ComponentCollisionChannel == ECC_MonsterSpell)
    {
        /* Sets the correct position of the hit component */

        UPrimitiveComponent* HitComponent = RadialDamage->ComponentHits[0].Component.Get(); /* Gets the hit component */

        ETrafficLightPosition Position = ETrafficLightPosition::Center; /* Default position is center */

        if(HitComponent == Hurtbox || HitComponent == CenterLightMesh)
        {
            Position = ETrafficLightPosition::Center;
        }
        else if(HitComponent == HurtboxRight || HitComponent == RightLightMesh)
        {
            Position = ETrafficLightPosition::Right;
        }
        else if(HitComponent == HurtboxLeft || HitComponent == LeftLightMesh)
        {
            Position = ETrafficLightPosition::Left;
        }

        TakeHurtboxDamageChangingLight(Damage, Position);
    }
    return Damage;
}


void ATrafficLightMW::TakeHurtboxDamageChangingLight(float Damage, const ETrafficLightPosition TrafficLightPosition)
{
    ChangeLight(TrafficLightPosition, ETrafficLight::Red);
    TakeHurtboxDamage(Damage);
}

void ATrafficLightMW::BeforeAttackEvent_Implementation()
{
    /* OVERRIDE THIS EVENT IN BP */
}
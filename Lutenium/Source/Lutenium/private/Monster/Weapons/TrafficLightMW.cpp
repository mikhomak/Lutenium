#include "Monster/Weapons/TrafficLightMW.h"
#include "Monster/Weapons/WeaponsUtils/TrafficLight.h"
#include "Monster/Weapons/WeaponsUtils/TrafficLightPosition.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Player/Missile.h"
#include "Player/PlayerPawn.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/MovementEffect/DragMovementEffect.h"
#include "Player/MovementEffect/EmpMovementEffect.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"

#define ECC_MonsterWPHurtbox ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Monster ECollisionChannel::ECC_GameTraceChannel2
#define ECC_MonsterSpell ECollisionChannel::ECC_GameTraceChannel3

ATrafficLightMW::ATrafficLightMW() : AMonsterWeapon()
{
    /* Creates two more hurtboxes*/
    const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
        EAttachmentRule::KeepRelative, false);
    HurtboxRight = CreateDefaultSubobject<USphereComponent>(TEXT("Hurt box right"));
    HurtboxRight->AttachToComponent(WeaponMesh, AttachmentTransformRules);
    HurtboxRight->SetCollisionProfileName(TEXT("MonsterWPHurtbox"));

    HurtboxLeft = CreateDefaultSubobject<USphereComponent>(TEXT("Hurt box left"));
    HurtboxLeft->AttachToComponent(WeaponMesh, AttachmentTransformRules);
    HurtboxLeft->SetCollisionProfileName(TEXT("MonsterWPHurtbox"));

    Health = 100.f;


    WeaponType = EMonsterWeaponType::TrafficLight;

    /* Creating Lights meshes */
    LeftLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Light Mesh"));
    RightLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Light Mesh"));
    CenterLightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Center Light Mesh"));

    LeftLightMesh->AttachToComponent(WeaponMesh, AttachmentTransformRules);
    RightLightMesh->AttachToComponent(WeaponMesh, AttachmentTransformRules);
    CenterLightMesh->AttachToComponent(WeaponMesh, AttachmentTransformRules);

    /* Creating arrows */
    LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Arrow"));
    RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Arrow"));
    CenterArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Center Arrow"));

    LeftArrow->AttachToComponent(WeaponMesh, AttachmentTransformRules);
    RightArrow->AttachToComponent(WeaponMesh, AttachmentTransformRules);
    CenterArrow->AttachToComponent(WeaponMesh, AttachmentTransformRules);

    /* Other */
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
            RightLightStatus = Light;
        break;
        case ETrafficLightPosition::Left:
            LeftLightStatus = Light;
        break;
        case ETrafficLightPosition::Center:
            CenterLightStatus = Light;
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
    FRotator PushDirection = GetActorRotation();
    switch(TrafficLightPosition)
    {
        case ETrafficLightPosition::Right:
            Position = RightLightMesh->GetComponentLocation();
            PushDirection = RightArrow->GetComponentRotation();
        break;
        case ETrafficLightPosition::Left:
            Position = LeftLightMesh->GetComponentLocation();
            PushDirection = LeftArrow->GetComponentRotation();
        break;
        case ETrafficLightPosition::Center:
            Position = CenterLightMesh->GetComponentLocation();
            PushDirection = CenterArrow->GetComponentRotation();
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
        Missile->ThrowMissile(PushDirection.Vector(), MissileThrowForce, bDefected);
        return;
    }

    /* Handles player overlap */
    /* If the player overlaps the red light, drags him away from the light */
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(Actor);
    if (TrafficLightStatus != ETrafficLight::Green && PlayerPawn)
    {
        /* RED */
        /* Emps the player */
        if(TrafficLightStatus == ETrafficLight::Red)
        {
            const FVector RandomRotation = FVector(
                FMath::RandRange(0.f, 1.f),
                FMath::RandRange(0.f, 1.f),
                FMath::RandRange(0.f, 1.f)
            );
            PlayerPawn->GetPlaneMovement()->EmpMovementEffect->Activate(RandomRotation, RedEmpForce);
        }
        /* YELLOW */
        /* Pushing the player away*/
        PlayerPawn->GetPlaneMovement()->DragMovementEffect->Activate(PlayerDragForce, PushDirection.Vector());
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
    /* Finding if there was a Monster Spell or Hurtbox in hit components of radial damage */
    /* If not, that means we hit only the mesh*/
    const FHitResult* HitResult = RadialDamage->ComponentHits.FindByPredicate([](const FHitResult& HitResult)
    {
       ECollisionChannel ComponentCollisionChannel = HitResult.Component.Get()->GetCollisionObjectType();
       return ComponentCollisionChannel == ECC_MonsterWPHurtbox || ComponentCollisionChannel == ECC_MonsterSpell;
    });


    /* If hit result was found, than we found in component hits */
    if(HitResult)
    {
        /* Sets the correct position of the hit component */
        UPrimitiveComponent* HitComponent = HitResult->Component.Get(); /* Gets the hit component */

        ETrafficLightPosition Position = ETrafficLightPosition::Center; /* Default position is center */
        ETrafficLight HitLightStatus = ETrafficLight::Green; /* Light status of the hit light to reduce the damage */
        bool bDidHitLightHurtbox = true; /* Checks if it has hit the hurtbox of the light or the normal hurtbox */

        if(HitComponent == Hurtbox || HitComponent == CenterLightMesh)
        {
            Position = ETrafficLightPosition::Center;
            HitLightStatus = CenterLightStatus;
        }
        else if(HitComponent == HurtboxRight || HitComponent == RightLightMesh)
        {
            Position = ETrafficLightPosition::Right;
            HitLightStatus = RightLightStatus;
        }
        else if(HitComponent == HurtboxLeft || HitComponent == LeftLightMesh)
        {
            Position = ETrafficLightPosition::Left;
            HitLightStatus = LeftLightStatus;
        }
        else
        {
            bDidHitLightHurtbox = false; /* we hit not the light hurtbox so we don't have to change the light */
        }

        /* if the player hit red light, do not damage */
        if(bDidHitLightHurtbox)
        {
            TakeHurtboxDamageChangingLight(Damage * (HitLightStatus != ETrafficLight::Red), Position);
        }
        else
        {
            TakeHurtboxDamage(Damage);
        }

        return Damage;
    }

    TakeMeshDamage(Damage);
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
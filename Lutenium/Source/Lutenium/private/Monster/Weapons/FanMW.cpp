#include "Monster/Weapons/FanMW.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Monster/Weapons/WeaponsUtils/MonsterWeaponType.h"
#include "Player/PlayerPawn.h"
#include "Player/Missile.h"
#include "Player/PlaneMovementComponent.h"
#include "Player/MovementEffect/DragMovementEffect.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"

AFanMW::AFanMW() : AMonsterWeapon()
{
    /* Setting up mesh collisions */
    ForwardWindCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Forward wind mesh"));
    BackwardWindCollision = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Backward wind mesh"));
    ForwardWindCollision->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);
    BackwardWindCollision->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);
    ForwardWindCollision->SetCollisionProfileName(TEXT("OverlapAll"));
    BackwardWindCollision->SetCollisionProfileName(TEXT("OverlapAll"));

    /* Forces */
    PushForce = 9000.f;
    PullForce = 9000.f;

    WindActiveTime = 5.f;

    Health = 100.f;

    WeaponType = EMonsterWeaponType::Fan;
}

void AFanMW::ExecuteAttack()
{
    bForwardWindActive = true;
    bBackwardWindActive = true;

    /* Invoking events for visuals n shit */
    OnActivateForwardWind(true);
    OnActivateForwardWind(false);

    /* After WindActiveTime deactivate winds */
    FTimerHandle WindTimeHandler;
    GetWorldTimerManager().SetTimer(WindTimeHandler, this, &AFanMW::ExecuteAttack, WindActiveTime, false);
}

/** Handles overlaped player and missile */
void AFanMW::WindTriggerOverlap(class AActor* OtherActor, bool bPush)
{
    /* Player has overlaped */
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
        FVector ForceDirection = PlayerPawn->GetActorLocation() - GetActorLocation();
        ForceDirection.Normalize();
        /* Pushing away player or pulling it towards */
        ForceDirection = bPush ? ForceDirection : ForceDirection * -1.f;
        const float Force = bPush ? PushForce : PullForce;
        PlayerPawn->GetPlaneMovement()->DragMovementEffect->Activate(Force, ForceDirection);
        return;
    }

    /* Missile has overlaped */
    AMissile* Missile = Cast<AMissile>(OtherActor);
    if(Missile)
    {
        /* Throws missile in the opposite direction */
        FVector Position = Missile->GetActorLocation() - Position;
        Position.Normalize();
        Missile->ThrowMissile(Missile->GetActorForwardVector() * -1, PushForce);
        /* Do not defected the missile */
        return;
    }
}

void AFanMW::WindTriggerOverlapEnd(class AActor* OtherActor)
{
    APlayerPawn* PlayerPawn = Cast<APlayerPawn>(OtherActor);
    if (PlayerPawn)
    {
        PlayerPawn->GetPlaneMovement()->DragMovementEffect->Deactivate();
    }
}

#include "Player/Missile.h"
#include "Player/PlayerPawn.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/DamageType.h"
#define ECC_Player ECollisionChannel::ECC_GameTraceChannel4

AMissile::AMissile()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Capcule collider"));
    SphereComponent->SetSimulatePhysics(false);
    RootComponent = SphereComponent;

    MissileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Missile Mesh"));
    MissileMesh->AttachToComponent(SphereComponent, FAttachmentTransformRules::KeepWorldTransform);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.f;

    RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force component"));
    RadialForceComponent->AttachToComponent(SphereComponent, FAttachmentTransformRules::KeepWorldTransform);
    RadialForceComponent->bIgnoreOwningActor = true;
    DefecteedGravityForceAmount = -50000000.f;
    DefecteedGravityRadius = 2000.f;
    RadialForceComponent->ForceStrength = DefecteedGravityForceAmount;
    RadialForceComponent->Radius = DefecteedGravityRadius;

    NonTargetSpeed = 5000.f;

    Damage = 150.f;
    ExplosionRadius = 2000.f;

    MissileLifeSpan = 20.f;

    DefecteedImpulseRadius = 2000.f;
    DefecteedImpulseForceAmount = 50000000.f;
}

void AMissile::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(MissileLifeSpan);
    RadialForceComponent->AddCollisionChannelToAffect(ECC_Player);
    RadialForceComponent->ToggleActive(); /* Disalbe radial force on the begging. Activating it in DefectMissile() */
}

void AMissile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMissile::SetTargetOrDirection(USceneComponent* Target, const FVector& ShootDirection)
{
    if (Target != nullptr)
    {
        ProjectileMovement->bIsHomingProjectile = true;
        ProjectileMovement->HomingTargetComponent = Target;
    }
    else
    {
        FVector Direction = ShootDirection;
        Direction.Normalize();
        ProjectileMovement->Velocity = Direction * NonTargetSpeed;
    }
}

void AMissile::ThrowMissile(FVector ThrownDirection, float ForceAmount, bool bDefectMissile)
{
    // Enemy could throw the missile and it will explode against the player
    if(!ThrownDirection.IsNormalized())
    {
        ThrownDirection.Normalize();
    }
    ProjectileMovement->bIsHomingProjectile = false;
    ProjectileMovement->Velocity = ThrownDirection * ForceAmount;

    /* Only execute defected logic if the missile was directly set to do it. EG - red traffic light*/
    if(bDefectMissile)
    {
        DefectMissile();
    }
}


void AMissile::EmpMissile()
{
    OnEmped(); /* Calls the event */
    /* Disables movement */
    ProjectileMovement->bIsHomingProjectile = false;
    ProjectileMovement->Velocity = FVector::ZeroVector;
    /* Adding gravity scale */
    ProjectileMovement->ProjectileGravityScale = EmpGravityScale;
}

void AMissile::DefectMissile()
{
    /* If it's already activated we don't want to activate it one more time */
    if(!RadialForceComponent->IsActive())
    {
        return;
    }
    SetLifeSpan(0); /* Removes the timer for life span */
    RadialForceComponent->ToggleActive();
    bIsDefected = true;
    GotDefected(); /* invokes the event */

    FTimerHandle DefectedImpuleTimer;
    GetWorldTimerManager().SetTimer(DefectedImpuleTimer, this, &AMissile::DefectedMissileImpulse, DefectedTime, false);
}

void AMissile::DefectedMissileImpulse()
{
    OnDefectedImpulse(); /** VFX, SFX and desotryin an actor*/
    RadialForceComponent->ForceStrength = DefecteedImpulseForceAmount;
    RadialForceComponent->Radius = DefecteedImpulseRadius;
    RadialForceComponent->FireImpulse();
}

void AMissile::Explode()
{
    OnExplode();
    // Spawn VFX in Bp!
    // Faster to do those shit in BP lol
    // Also bind this function to Overlap and OnDestroy event
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage,
                                        GetActorLocation(), ExplosionRadius,
                                        UDamageType::StaticClass(), TArray<AActor*>(),
                                        this, (AController*)GetOwner(),
                                        true, ECC_Visibility);
}
#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#define ECC_MonsterWPHurtbox ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Monster ECollisionChannel::ECC_GameTraceChannel2
#define ECC_MonsterSpell ECollisionChannel::ECC_GameTraceChannel3

AMonsterWeapon::AMonsterWeapon()
{

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = WeaponMesh;

    // set the radius of the hurtbox in bp
    const FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
        EAttachmentRule::KeepRelative, false);
    Hurtbox = CreateDefaultSubobject<USphereComponent>(TEXT("Hurt box"));
    Hurtbox->AttachToComponent(WeaponMesh, AttachmentTransformRules);
    Hurtbox->SetCollisionProfileName(TEXT("MonsterWPHurtbox"));
    PrimaryActorTick.bCanEverTick = true;

    /* Damage & Health */
    Health = 100.f;
    MassInKgAfterDetach = 55000.f;
    MeshDamageReduction = 0.4f;
    InvincibilityTime = 0.1f;
    bShouldFlash = true;

    /* Attack */
    TimeBeforeAttack = 1.f;
    RepeatAttackTime = 2.5f;
    LevelUpgrade = 0;

    /* Cooldown */
    bCanAttack = true;
    CooldownTime = 10.f;

    bDebugDetach=false;
}

void AMonsterWeapon::BeginPlay()
{
    Super::BeginPlay();

    // Material
    UMaterialInterface* Material = WeaponMesh->GetMaterial(0);
    FlashParameterName = "Flashing Hit Alpha Lerp";

    if(Material)
    {
        MainMaterialInstance = WeaponMesh->CreateDynamicMaterialInstance(0, Material);
    }
}

float AMonsterWeapon::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if(!DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
    {
        return 0.f;
    }
    const auto RadialDamage = (FRadialDamageEvent*)&DamageEvent;
    /* getting collision object type to difirentatite mesh from other parts of the weapon */
    const FHitResult* HitResult = RadialDamage->ComponentHits.FindByPredicate([](const FHitResult& HitResult)
    {
       ECollisionChannel ComponentCollisionChannel = HitResult.Component.Get()->GetCollisionObjectType();
       return ComponentCollisionChannel == ECC_MonsterWPHurtbox || ComponentCollisionChannel == ECC_MonsterSpell;
    });
    /* It was hurtbox */
    if(HitResult)
    {
        TakeHurtboxDamage(DamageAmount);
        return DamageAmount;
    }
    /* It was mesh */
    TakeMeshDamage(DamageAmount);
    return DamageAmount;
}

void AMonsterWeapon::TakeMeshDamage(float Damage)
{
    OnTakeDamage(Damage * MeshDamageReduction);
}

void AMonsterWeapon::TakeHurtboxDamage(float Damage)
{
    OnTakeDamage(Damage);
}


void AMonsterWeapon::OnTakeDamage(float Damage)
{
    /* Do nothign if the weapon is invincible right now */
    if(!CanBeDamaged())
    {
        return;
    }

    /* Making weapon invincible for sometime */
    SetCanBeDamaged(false);

    Health -= Damage;

    // Apply damage to the monster itself
    if(MonsterPawn)
    {
        MonsterPawn->TakeNonDirectDamage(Damage, true);
    }

    // ya die if ya die
    if(Health < 0.f)
    {
        DieEvent();
        Die();
    }
    else /* Removing Invincibility only if the weapon hasn't died yet */
    {
        if(bShouldFlash && MainMaterialInstance != nullptr)
        {
            MainMaterialInstance->SetScalarParameterValue(FlashParameterName, 1.f);
        }
        TakeDamageEvent(Damage);
        FTimerHandle InvincibilityTimer;
        GetWorldTimerManager().SetTimer(InvincibilityTimer, this, &AMonsterWeapon::InvincibilityEnd, InvincibilityTime, false);
    }
}

void AMonsterWeapon::Die()
{
    if(bDebugDetach)
    {
        return;
    }
    const FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
    WeaponMesh->DetachFromComponent(DetachmentTransformRules);
    // make this shit fall!
    WeaponMesh->SetSimulatePhysics(true);
    WeaponMesh->SetMassOverrideInKg(FName(), MassInKgAfterDetach, true);
    WeaponMesh->SetCollisionProfileName(TEXT("BlockAll"));

    // disable hurtbox collision so we couldnt target it with the missiles
    Hurtbox->SetCollisionProfileName(TEXT("NoCollision"));

    //Removes the refernse of the weapons and upgrades all the other ones
    if(MonsterPawn)
    {
        MonsterPawn->LooseWeapon(WeaponType);
    }
}

void AMonsterWeapon::DoAttack()
{
    if(!bCanAttack)
    {
        return;
    }
    CurrentExecutedAttacks = 0;
    /* Invoking event to add some stuff in BP(sfx, vfx, so on...)*/
    BeforeAttackEvent();

    /* Delays actual attack */
    FTimerHandle AttackHandler;
    GetWorldTimerManager().SetTimer(AttackHandler, this, &AMonsterWeapon::ExecuteAttack, TimeBeforeAttack, false);

    /* Sets cooldown */
    bCanAttack = false;
    FTimerHandle CooldownTimer;
    GetWorldTimerManager().SetTimer(CooldownTimer, this, &AMonsterWeapon::CooldownEnd, CooldownTime, false);
}

void AMonsterWeapon::ExecuteAttack()
{
    /* DO NOTHING */
    /* VIRTUAL METHOD */
}

void AMonsterWeapon::BeforeAttackEvent_Implementation()
{
    /* DO NOTHING */
    /* VIRTUAL METHOD */
}

void AMonsterWeapon::SpecificUpgrade(int32 Level)
{
    /* DO NOTHING */
    /* VIRTUAL METHOD */
}
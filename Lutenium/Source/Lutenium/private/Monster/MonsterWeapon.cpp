#include "Monster/Weapons/MonsterWeapon.h"
#include "Monster/EnemyMonsterPawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

AMonsterWeapon::AMonsterWeapon()
{

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = WeaponMesh;
    // set the radius of the hurtbox in bp
    Hurtbox = CreateDefaultSubobject<USphereComponent>(TEXT("Hurt box"));
    Hurtbox->AttachToComponent(WeaponMesh, FAttachmentTransformRules::KeepWorldTransform);
    Hurtbox->SetCollisionProfileName(TEXT("MonsterWPHurtbox"));
    PrimaryActorTick.bCanEverTick = true;

    Health = 100.f;
    MassInKgAfterDetach = 55000.f;

    TimeBeforeAttack = 0;

    bDebugDetach=false;
}

float AMonsterWeapon::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
    Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    OnTakeDamage(Damage);
    return Damage;
}


void AMonsterWeapon::OnTakeDamage(float Damage)
{
    Health -= Damage;

    // Apply damage to the monster itself
    if(MonsterPawn)
    {
        MonsterPawn->TakeNonDirectDamage(Damage);
    }

    // ya die if ya die
    if(Health < 0.f)
    {
        DieEvent();
        Die();
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

    //disable hurtbox collision so we couldnt target it with the missiles
    Hurtbox->SetCollisionProfileName(TEXT("NoCollision"));

    //Removes the refernse of the weapons and upgrades all the other ones
    if(MonsterPawn)
    {
        MonsterPawn->LooseWeapon(WeaponType);
    }
}

void AMonsterWeapon::DoAttack()
{
    /* Invoking event to add some stuff in BP(sfx, vfx, so on...)*/
    BeforeAttackEvent();

    /* Delays actual attack */
    FTimerHandle AttackHandler;
    GetWorldTimerManager().SetTimer(AttackHandler, this, &AMonsterWeapon::ExecuteAttack, TimeBeforeAttack, false);
}

void AMonsterWeapon::ExecuteAttack()
{
    /* DO NOTHING */
    /* VIRTUAL METHOD */
}
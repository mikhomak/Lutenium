#include "../../public/Monster/MonsterWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

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
}

float AMonsterWeapon::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser)
{
    OnTakeDamage(Damage);
    return Damage;
}


void AMonsterWeapon::OnTakeDamage(float Damage)
{
    Health -= Damage;
    if(Health < 0.f)
    {
        DieEvent();
        Die();
    }
}

void AMonsterWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void AMonsterWeapon::Die()
{
    const FDetachmentTransformRules DetachmentTransformRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
    WeaponMesh->DetachFromComponent(DetachmentTransformRules);
}
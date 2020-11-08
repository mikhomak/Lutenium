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
}


void AMonsterWeapon::OnTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
    AActor* DamageCauser)
{
}

void AMonsterWeapon::BeginPlay()
{
    Super::BeginPlay();
}

#pragma once
UENUM (BlueprintType)
enum class EMissileTargetHit : uint8
{
    MonsterWPHurtbox UMETA(DisplayName = "Monster Weapon hurtbox"),
    Monster UMETA(DisplayName = "Monster"),
    NoHit UMETA(DisplayName = "No Hit"),
};

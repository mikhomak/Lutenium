#pragma once
UENUM (BlueprintType)
enum class EMonsterWeaponType : uint8
{
    Fan UMETA(DisplayName = "Fan"),
    Pipe UMETA(DisplayName = "Pipe"),
    TrafficLight UMETA(DisplayName = "Traffic Light"),
    Siren UMETA(DisplayName = "Siren"),
};

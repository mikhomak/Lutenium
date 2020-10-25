#pragma once
UENUM (BlueprintType)
enum class ETrafficLight : uint8
{
    Red UMETA(DisplayName = "Red"),
    Yellow UMETA(DisplayName = "Yellow"),
    Green UMETA(DisplayName = "Green"),
};

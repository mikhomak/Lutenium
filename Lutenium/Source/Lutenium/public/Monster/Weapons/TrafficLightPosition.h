#pragma once
UENUM (BlueprintType)
enum class ETrafficLightPosition : uint8
{
    Right UMETA(DisplayName = "Right"),
    Left UMETA(DisplayName = "Left"),
    Center UMETA(DisplayName = "Center"),
};

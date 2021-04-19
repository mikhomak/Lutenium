
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/PlaneMovementComponent.h"
#include "PlayerPlaneMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UPlayerPlaneMovementComponent final : public UPlaneMovementComponent
{

public:

	GENERATED_BODY()

    UPlayerPlaneMovementComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    virtual void BeginPlay() override;


    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    class APlayerPawn* PlayerPawn;

    FORCEINLINE UFUNCTION(Category = "General")
    void SetPawn(class APlayerPawn* Pawn)
    {
    	OwnerPawn = (APawn*)Pawn;
    	PlayerPawn = Pawn;
    }


    virtual void ThrustInput(float Val) override;

    virtual void Thrusting(float InputVal) override;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    float TimeBetweenDoubleThrustInputToEnterTravelMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    float TravelModeMaxAccelerationMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    float TravelModeAircontrolMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    bool bCanEnterTravelMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    bool bIsInTravelMode;

    UFUNCTION(BlueprintCallable, Category="Travel Mode")
    void ActivateTravelMode();

    UFUNCTION(BlueprintCallable, Category="Travel Mode")
    void DeactivateTravelMode();


    UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
    void DashInput();


    UPROPERTY(Category = "Dash", EditAnywhere)
    int MaxDashes;

    UPROPERTY(Category = "Dash", EditAnywhere)
    float DashCooldown;


    // ------------------------------------------------------------------
    // DASH
    // ------------------------------------------------------------------
    UPROPERTY(Category = Dash, EditDefaultsOnly)
    float DashImpactForce;

    int DashesLeft;

    bool bCanDash;

    void ResetDashCooldown();


};
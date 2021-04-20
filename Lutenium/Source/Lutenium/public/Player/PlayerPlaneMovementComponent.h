
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/PlaneMovementComponent.h"
#include "InputActionValue.h"
#include "PlayerPlaneMovementComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTravelModeActivateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTravelModeDeactivateSignature);



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


    virtual void ThrustInput(const FInputActionValue& Value) override;

    virtual void Thrusting(float InputVal) override;

    UFUNCTION(Category = "Control")
    void ReleasedThrustInput();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    float TimeBetweenDoubleThrustInputToEnterTravelMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode", meta = (ClampMin = "1", UIMin = "1", ClampMax = "2", UIMax = "2"))
    float TravelModeMaxAccelerationMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode", meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
    float TravelModeAircontrolMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    bool bCanEnterTravelMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    bool bIsInTravelMode;

    UFUNCTION(BlueprintCallable, Category="Travel Mode")
    void ActivateTravelMode();

    UFUNCTION(BlueprintCallable, Category="Travel Mode")
    void DeactivateTravelMode();

    FORCEINLINE UFUNCTION(Category="Travel Mode")
    void ResetEnteringTravelMode() 
    {
        if(bIsInTravelMode == false)
        {
            bCanEnterTravelMode = false;
            TravelModeThurstActivations = 0;
        }
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel mode")
    int32 TravelModeThurstActivations;

    UPROPERTY(BlueprintAssignable, Category="Travel Mode")
    FOnTravelModeActivateSignature OnTravelModeActivate;


    UPROPERTY(BlueprintAssignable, Category="Travel Mode")
    FOnTravelModeDeactivateSignature OnTravelModeDeactivate;

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
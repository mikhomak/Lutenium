
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/PlaneMovementComponent.h"
#include "InputActionValue.h"
#include "PlayerPlaneMovementComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTravelModeActivateSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTravelModeDeactivateSignature);


/**
 * Enhanced PlaneMovementComponent with new features, such as:
 *  1) Travel Mode
 *  2) Dashes
 * Those feature don't need to be in PlaneMovementComponent because they are more specific
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LUTENIUM_API UPlayerPlaneMovementComponent final : public UPlaneMovementComponent
{

public:

	GENERATED_BODY()

    UPlayerPlaneMovementComponent();

    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    virtual void BeginPlay() override;


    /***
    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    class APlayerPawn* PlayerPawn;

    FORCEINLINE UFUNCTION(Category = "General")
    void SetPawn(class APlayerPawn* Pawn)
    {
    	OwnerPawn = (APawn*)Pawn;
    	PlayerPawn = Pawn;
    }

    // ------------------------------------------------------------------
    // Travel Mode
    // ------------------------------------------------------------------
    
    /**
     * Overriding ThrustInput method to savely deactivate travel mode in case if the player presses "thrust back"
     */
    virtual void ThrustInput(const FInputActionValue& Value) override;

    /**
     * On entering 
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode", meta = (ClampMin = "1", UIMin = "1", ClampMax = "2", UIMax = "2"))
    float TravelModeMaxAccelerationMultiplier;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode", meta = (ClampMin = "0", UIMin = "0", ClampMax = "1", UIMax = "1"))
    float TravelModeAircontrolMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    bool bIsInTravelMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    bool bCanTravelMode;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Travel Mode")
    float TimeToBeAbleToDeactivateTravelMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Travel Mode")
    bool bIsAbleToDeactivateTravelMode;

    FORCEINLINE UFUNCTION(Category = "Travel Mode")
    void CanDeactivateTravelMode()
    {
        bIsAbleToDeactivateTravelMode = true;
    }

    UFUNCTION(BlueprintCallable, Category="Travel Mode")
    void ActivateTravelMode();

    UFUNCTION(BlueprintCallable, Category="Travel Mode")
    void DeactivateTravelMode();


    UPROPERTY(BlueprintAssignable, Category="Travel Mode")
    FOnTravelModeActivateSignature OnTravelModeActivate;

    UPROPERTY(BlueprintAssignable, Category="Travel Mode")
    FOnTravelModeDeactivateSignature OnTravelModeDeactivate;


    // ------------------------------------------------------------------
    // DASH
    // ------------------------------------------------------------------

    UFUNCTION(BlueprintCallable, Category = "Input", meta = (AdvancedDisplay = "2"))
    void DashInput();


    UPROPERTY(Category = "Dash", EditAnywhere)
    int MaxDashes;

    UPROPERTY(Category = "Dash", EditAnywhere)
    float DashCooldown;

    UPROPERTY(Category = Dash, EditDefaultsOnly)
    float DashImpactForce;

    int DashesLeft;

    bool bCanDash;

    void ResetDashCooldown();


};
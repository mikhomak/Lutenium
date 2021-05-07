#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "LuteniumPlayerController.generated.h"

/** 
 * Player controller with enhanced input system
 */
UCLASS(config=Game, BlueprintType, Blueprintable)
class ALuteniumPlayerController : public APlayerController
{
	GENERATED_BODY()

	// ------------------------------------------------------------------
	// Pause Menu
	// ------------------------------------------------------------------
public:
	/**
	 * Activates or deactivates the pause menu widget
	 * Checks whenever bIsPauseMenuActive to activate/deactivate menu widget
	 * @param bActivate - Activate pause menu?
	 */
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void ActivatePauseMenu(bool bActivate);

	/**
	 * Toggles the pause menu to acitvate/deactivate paramause mene
	 * Checks by bIsPauseMenuActive
	 * Invokes AcitvatePauseMenu()
	 */
	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void TogglePauseMenu();

	/**
	 * Events happens when the menu has toggled
	 * @param bActivate - Is current menu active or deactivated?
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void OnTogglePauseMenu(const bool bActive);

	/**
	 * Input action for the pause menu
	 * Uses EnhancedInputSystem
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pause Menu")
	class UInputAction* PauseMenuInputAction;

	/**
	 * Is Pause Menu currnetly active?
	 * Updates in ActivatePauseMenu()
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pause Menu")
	bool bIsPauseMenuActive;

	/**
	 * Pause menu widget
	 * Gets activated/deactivated in ActivatePauseMenu()
	 */
    UPROPERTY(BlueprintReadOnly, Category = "Pause Menu")
    class UUserWidget* PauseMenuWidget;

    /**
     * BP class of the variable PauseMenuWidget
     */
	UPROPERTY(EditDefaultsOnly, Category = "Pause Menu")
    TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	/**
	 * Pause Menu Input Mapping
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Mapping")
	class UInputMappingContext* PauseMenuInputMapping;

	/**
	 * In Game Menu Mapping
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input|Mapping")
	class UInputMappingContext* GameInputMapping;


	/**
	 * Sets input mapping for the this controller previously clearing all mappings
	 */
	UFUNCTION(BlueprintCallable, Category="Input|Mapping")
	void SetClearInputMapping(class UInputMappingContext* InputMapping);
protected:
	
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
};

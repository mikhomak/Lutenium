#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "LuteniumPlayerController.generated.h"



USTRUCT(BlueprintType)
struct FPlayerPauseGame
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pause")
	bool bShouldUpdatePausing;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pause")
	bool bPauseTheGame;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pause")
	bool bShowMouseCursor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pause")
	bool bUseDefaultPauseMapping;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Pause")
	class UInputMappingContext* NewInputMapping;

	FPlayerPauseGame(){};
};

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
	 * Uses ResumeGame()
	 * Uses OnPauseTheGame()
	 *
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
	 * 
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
	 *
	 * @param InputMapping - new input mapping to set to the current input system
	 */
	UFUNCTION(BlueprintCallable, Category="Input|Mapping")
	void SetClearInputMapping(class UInputMappingContext* InputMapping);

	// ------------------------------------------------------------------
	// Pausing the game
	// ------------------------------------------------------------------
	
	/**
	 * Pausing the game
	 * Not necessarily to show the pause menu (like showing widget tutorials and such)
	 * By using FPlayerPauseGame we can update the settings inside the controller
	 * However it's better to use this funciton only to update the player-related functionallity (input, etc..)
	 * And do the rest in GameMode/Instance
	 *
	 * @param PlayerPauseGameSettings to update the setting for the pausing(like disabling input and such)
	 */
	UFUNCTION(BlueprintCallable, Category = "Pausing")
	void OnPauseTheGame(const FPlayerPauseGame& PlayerPauseGameSettings);


	/**
	 * Resuming game
	 * Forcing SetPause(false)
	 * Force disabling the cursour
	 * And sets the Input Mapping to GameInputMapping
	 */
	UFUNCTION(BlueprintCallable, Category = "Pausing")
	void ResumeGame();

protected:
	
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
};

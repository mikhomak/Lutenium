#include "Player/LuteniumPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"

void ALuteniumPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (PauseMenuWidgetClass)
    {
        PauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
    }

}

void ALuteniumPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    // Binding controller related actions
    if(UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if(PauseMenuInputAction)
        {
            PlayerEnhancedInputComponent->BindAction(PauseMenuInputAction, ETriggerEvent::Completed, this, &ALuteniumPlayerController::TogglePauseMenu);
        }
    }

}

void ALuteniumPlayerController::TogglePauseMenu()
{
    ActivatePauseMenu(!bIsPauseMenuActive);   
}

void ALuteniumPlayerController::ActivatePauseMenu(bool bActivate)
{
    // safety check
    if(PauseMenuWidget == nullptr)
    {
        return;
    }

    // activate pause menu
    if(bActivate && !bIsPauseMenuActive)
    {
        // Pause settings
        FPlayerPauseGame PauseMenuSettings;
        PauseMenuSettings.bShouldUpdatePausing = true;
        PauseMenuSettings.bPauseTheGame = true;
        OnPauseTheGame(PauseMenuSettings);

        PauseMenuWidget->AddToViewport(1);
        bIsPauseMenuActive = true;
        OnTogglePauseMenu(bActivate);
    }
    // resuming pause menu
    else if(!bActivate && bIsPauseMenuActive)
    {
        PauseMenuWidget->RemoveFromParent();
        bIsPauseMenuActive = false;
        ResumeGame();
        OnTogglePauseMenu(bActivate);
    }
}


void ALuteniumPlayerController::SetClearInputMapping(class UInputMappingContext* InputMapping)
{
    // safety check
    if(InputMapping == nullptr)
    {
        return;
    }

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
    {
        Subsystem->ClearAllMappings();
        Subsystem->AddMappingContext(InputMapping, 1);
    }
}


void ALuteniumPlayerController::OnPauseTheGame(FPlayerPauseGame& PlayerPauseGameSettings)
{

    // Pause/resume the game?
    if(PlayerPauseGameSettings.bShouldUpdatePausing)
    {
        SetPause(PlayerPauseGameSettings.bPauseTheGame);
    }
    
    // Cursor
    bShowMouseCursor = PlayerPauseGameSettings.bShowMouseCursor;

    // Input mapping
    if(PlayerPauseGameSettings.bUseDefaultPauseMapping)
    {
        SetClearInputMapping(PauseMenuInputMapping);
    }
    else
    {
        SetClearInputMapping(PlayerPauseGameSettings.NewInputMapping);
    }


}

void ALuteniumPlayerController::ResumeGame()
{
    SetPause(false);
    bShowMouseCursor = false;
    SetClearInputMapping(GameInputMapping);
}
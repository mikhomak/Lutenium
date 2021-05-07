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
        SetPause(true);
        PauseMenuWidget->AddToViewport(1);
        bShowMouseCursor = true;
        SetClearInputMapping(PauseMenuInputMapping);
        bIsPauseMenuActive = true;
        OnTogglePauseMenu(bActivate);
    }
    else if(!bActivate && bIsPauseMenuActive)
    {
        SetPause(false);
        PauseMenuWidget->RemoveFromParent();
        bShowMouseCursor = false;
        bIsPauseMenuActive = false;
        SetClearInputMapping(GameInputMapping);
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
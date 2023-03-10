// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"

#include "MenuInterface.h"

#include "Components/Button.h"

void UInGameMenu::Activate(IMenuInterface* _menuInterface)
{
	menuInterface = _menuInterface;

	AddToViewport();

	auto* playerController = GetWorld()->GetFirstPlayerController();

	if (playerController != nullptr)
	{
		FInputModeUIOnly inputMode;

		bIsFocusable = true;
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		playerController->SetInputMode(inputMode);
		playerController->SetShowMouseCursor(true);
	}
}

void UInGameMenu::Deactivate()
{
	menuInterface = nullptr;

	RemoveFromViewport();

	auto* playerController = GetWorld()->GetFirstPlayerController();

	if (playerController != nullptr)
	{
		FInputModeGameOnly inputMode;

		playerController->SetInputMode(inputMode);
		playerController->SetShowMouseCursor(false);
	}
}

bool UInGameMenu::Initialize()
{
	if (Super::Initialize() == false ||
		leaveButton == nullptr ||
		cancelInGameMenuButton == nullptr)
		return false;

	leaveButton->OnClicked.AddDynamic(this, &UInGameMenu::LeaveGame);
	cancelInGameMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::Close);

	return true;
}

void UInGameMenu::NativeDestruct()
{
	Deactivate();

	Super::NativeDestruct();
}

void UInGameMenu::LeaveGame()
{
	if (menuInterface != nullptr)
	{
		if (leaveButton != nullptr)
			menuInterface->Leave();
	}
}

void UInGameMenu::Close()
{
	Deactivate();
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "MenuInterface.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

void UMainMenu::Activate(IMenuInterface* _menuInterface)
{
	menuInterface = _menuInterface;

	AddToViewport();

	auto* playerController = GetWorld()->GetFirstPlayerController();

	if (playerController != nullptr)
	{
		FInputModeUIOnly inputMode;

		inputMode.SetWidgetToFocus(TakeWidget());
		inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		playerController->SetInputMode(inputMode);
		playerController->SetShowMouseCursor(true);
	}
}

void UMainMenu::Deactivate()
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

bool UMainMenu::Initialize()
{
	if (Super::Initialize() == false ||
		hostButton == nullptr ||
		joinMenuButton == nullptr ||
		joinButton == nullptr ||
		cancleButton == nullptr)
		return false;

	hostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	joinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	cancleButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

	return true;
}

void UMainMenu::NativeDestruct()
{
	Super::NativeDestruct();
}

void UMainMenu::HostServer()
{
	if (menuInterface != nullptr)
		menuInterface->Host();
}

void UMainMenu::OpenJoinMenu()
{
	if (menuSwitcher != nullptr)
	{
		if (joinMenu != nullptr)
			menuSwitcher->SetActiveWidget(joinMenu);
	}
}

void UMainMenu::BackToMainMenu()
{
	if (menuSwitcher != nullptr)
	{
		if (mainMenu != nullptr)
			menuSwitcher->SetActiveWidget(mainMenu);
	}
}
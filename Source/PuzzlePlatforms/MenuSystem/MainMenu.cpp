// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "MenuInterface.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

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
		cancleJoinMenuButton == nullptr)
		return false;

	hostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	joinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	joinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	cancleJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

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

void UMainMenu::JoinServer()
{
	if (menuInterface != nullptr)
	{
		if (inputIPAddress != nullptr)
		{
			const auto& ip = inputIPAddress->GetText().ToString();

			menuInterface->Join(ip);
		}
	}
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
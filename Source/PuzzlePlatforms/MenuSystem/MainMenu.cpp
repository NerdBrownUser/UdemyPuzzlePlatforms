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
		FInputModeGameAndUI inputMode;

		bIsFocusable = true;
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

void UMainMenu::OpenQuitMenu()
{
	if (menuSwitcher != nullptr)
	{
		if (quitMenu != nullptr)
			menuSwitcher->SetActiveWidget(quitMenu);
	}
}

bool UMainMenu::Initialize()
{
	if (Super::Initialize() == false ||
		hostButton == nullptr ||
		joinMenuButton == nullptr ||
		joinButton == nullptr ||
		cancelJoinMenuButton == nullptr ||
		quitButton == nullptr ||
		cancelQuitMenuButton == nullptr)
		return false;

	hostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	joinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	joinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	cancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	quitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	cancelQuitMenuButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

	return true;
}

void UMainMenu::NativeDestruct()
{
	Deactivate();

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

void UMainMenu::QuitGame()
{
	if (menuInterface != nullptr)
		menuInterface->QuitGame();
}
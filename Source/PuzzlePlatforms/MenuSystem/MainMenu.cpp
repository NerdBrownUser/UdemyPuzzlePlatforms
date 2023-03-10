// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"
#include "MenuInterface.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> serverRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (serverRowBPClass.Class != nullptr)
		serverRowClass = serverRowBPClass.Class;
}

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

void UMainMenu::AddServerRow(const FString& serverName)
{
	if (serverList != nullptr && serverRowClass != nullptr)
	{
		UServerRow* newServer = CreateWidget<UServerRow>(this, serverRowClass);

		newServer->serverName->SetText(FText::FromString(serverName));

		serverList->AddChild(newServer);
	}
}

void UMainMenu::ClearServerList()
{
	if (serverList != nullptr)
	{
		serverList->ClearChildren();
	}
}

bool UMainMenu::Initialize()
{
	if (Super::Initialize() == false ||
		hostButton == nullptr ||
		joinMenuButton == nullptr ||
		joinButton == nullptr ||
		refreshButton == nullptr ||
		cancelJoinMenuButton == nullptr ||
		quitButton == nullptr ||
		cancelQuitMenuButton == nullptr)
		return false;

	hostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	joinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	joinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	refreshButton->OnClicked.AddDynamic(this, &UMainMenu::RefreshServerList);
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

	}
}

void UMainMenu::OpenJoinMenu()
{
	if (menuSwitcher != nullptr)
	{
		if (joinMenu != nullptr)
		{
			menuInterface->RefreshServerList();

			menuSwitcher->SetActiveWidget(joinMenu);
		}
	}
}

void UMainMenu::RefreshServerList()
{
	if (menuInterface != nullptr)
		menuInterface->RefreshServerList();
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
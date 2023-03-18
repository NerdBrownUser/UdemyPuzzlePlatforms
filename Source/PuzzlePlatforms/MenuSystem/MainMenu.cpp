// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"

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

	RemoveFromParent();

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

void UMainMenu::AddServerRow(const FServerData& serverData)
{
	if (serverList != nullptr && serverRowClass != nullptr)
	{
		UServerRow* newServer = CreateWidget<UServerRow>(this, serverRowClass);

		newServer->serverName->SetText(FText::FromString(serverData.name));
		newServer->hostUserName->SetText(FText::FromString(serverData.hostUserName));
		newServer->SetPlayerCounts(serverData.currentPlayers, serverData.maxPlayers);
		newServer->SetParentWidget(this, serverList->GetChildrenCount());

		serverList->AddChild(newServer);
	}
}

void UMainMenu::ClearServerList()
{
	if (serverList != nullptr)
	{
		selectedIndex.Reset();

		serverList->ClearChildren();
	}
}

void UMainMenu::SelectIndex(uint32 index)
{
	if (selectedIndex.IsSet() == true)
	{
		auto* child = Cast<UServerRow>(serverList->GetChildAt(selectedIndex.GetValue()));

		if (child != nullptr)
			child->ChangeTextColor(1.0f, 1.0f, 1.0f, 1.0f);
	}

	auto* selectedChild = Cast<UServerRow>(serverList->GetChildAt(index));

	if (selectedChild != nullptr)
		selectedChild->ChangeTextColor(0.0f, 1.0f, 0.0f, 1.0f);

	selectedIndex = index;

	UE_LOG(LogTemp, Warning, _T("Selected index is %d"), selectedIndex.GetValue());
}

bool UMainMenu::Initialize()
{
	if (Super::Initialize() == false ||
		hostButton == nullptr ||
		joinMenuButton == nullptr ||
		quitMenuButton == nullptr ||
		confirmButton == nullptr ||
		cancelHostMenuButton == nullptr ||
		joinButton == nullptr ||
		refreshButton == nullptr ||
		cancelJoinMenuButton == nullptr ||
		quitButton == nullptr ||
		cancelQuitMenuButton == nullptr)
		return false;

	hostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);
	joinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	quitMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenQuitMenu);
	confirmButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	cancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
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
	{
		if (lobbyName->GetText().IsEmpty() != true)
			menuInterface->Host(lobbyName->GetText());
	}
}

void UMainMenu::JoinServer()
{
	if (selectedIndex.IsSet() == true)
	{
		UE_LOG(LogTemp, Warning, _T("Selected Index %d"), selectedIndex.GetValue());

		if (menuInterface != nullptr)
			menuInterface->Join(selectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, _T("Selected index not set"));
	}
}

void UMainMenu::OpenHostMenu()
{
	if (menuSwitcher != nullptr)
	{
		ClearServerList();
		
		if (hostMenu != nullptr)
			menuSwitcher->SetActiveWidget(hostMenu);
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
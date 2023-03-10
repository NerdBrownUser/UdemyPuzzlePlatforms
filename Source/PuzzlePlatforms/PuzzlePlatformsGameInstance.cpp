// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> menuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (menuBPClass.Class != nullptr)
		menuClass = menuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> inGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (inGameMenuBPClass.Class != nullptr)
		inGameMenuClass = inGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, _T("Found class %s"), *menuClass->GetName());
	UE_LOG(LogTemp, Warning, _T("Found class %s"), *inGameMenuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	if (menuClass != nullptr)
	{
		mainMenu = CreateWidget<UMainMenu>(this, menuClass, _T("MenuClass"));

		if (mainMenu != nullptr)
			mainMenu->Activate(this);
	}
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (inGameMenuClass != nullptr)
		inGameMenu = CreateWidget<UInGameMenu>(this, inGameMenuClass, _T("InGameMenuClass"));
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, _T("Hosting"));

		if (GWorld != nullptr)
			GWorld->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}

void UPuzzlePlatformsGameInstance::Join(const FString& ipAddress)
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, FString::Printf(_T("Joining %s"), *ipAddress));
	
		auto* playerController = GetFirstLocalPlayerController();

		if (playerController != nullptr)
			playerController->ClientTravel(ipAddress, ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzlePlatformsGameInstance::OpenInGameMenu()
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, _T("Open In Game Menu"));

		if (inGameMenu != nullptr)
			inGameMenu->Activate(this);
	}
}

void UPuzzlePlatformsGameInstance::Leave()
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, _T("Leave"));

		auto* playerController = GetFirstLocalPlayerController();

		if (playerController != nullptr)
			playerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzlePlatformsGameInstance::QuitGame()
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, _T("Quit"));

		auto* playerController = GetFirstLocalPlayerController();

		if (playerController != nullptr)
			playerController->ConsoleCommand("Quit");
	}
}
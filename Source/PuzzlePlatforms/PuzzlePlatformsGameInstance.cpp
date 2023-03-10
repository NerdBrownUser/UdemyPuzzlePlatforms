// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "MenuSystem/MainMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> menuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (menuBPClass.Class != nullptr)
	{
		menuClass = menuBPClass.Class;
	}
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, _T("Found class %s"), *menuClass->GetName());
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (menuClass != nullptr)
	{
		mainMenu = CreateWidget<UMainMenu>(this, menuClass, _T("MenuClass"));

		if (mainMenu != nullptr)
			mainMenu->Activate(this);
	}
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, _T("Hosting"));

		if (GWorld != nullptr)
		{
			GWorld->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");

			mainMenu->Deactivate();
		}
	}
}

void UPuzzlePlatformsGameInstance::Join(const FString& ipAddress)
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, FString::Printf(_T("Joining %s"), *ipAddress));
	
		auto* playerController = GetFirstLocalPlayerController();

		if (playerController != nullptr)
		{
			playerController->ClientTravel(ipAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}
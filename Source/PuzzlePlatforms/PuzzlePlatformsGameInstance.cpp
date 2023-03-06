// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

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
		UUserWidget* mainMenu = CreateWidget<UUserWidget>(this, menuClass, _T("MenuClass"));

		mainMenu->AddToViewport();

		auto* playerController = GetFirstLocalPlayerController();

		if (playerController != nullptr)
		{
			FInputModeUIOnly inputMode;

			inputMode.SetWidgetToFocus(mainMenu->TakeWidget());
			inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			playerController->SetInputMode(inputMode);

			playerController->SetShowMouseCursor(true);
		}
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
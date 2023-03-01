// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{

}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
}

void UPuzzlePlatformsGameInstance::Host()
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, _T("Hosting"));
	}

	if (GWorld != nullptr)
	{
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
		{
			playerController->ClientTravel(ipAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}
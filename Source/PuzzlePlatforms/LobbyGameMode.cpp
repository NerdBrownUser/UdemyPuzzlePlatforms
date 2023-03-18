// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameMapsSettings.h"

#include "PuzzlePlatformsGameInstance.h"

#include "TimerManager.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerCount++;

	UE_LOG(LogTemp, Warning, _T("%d Player(s) in this lobby."), PlayerCount);

	if (PlayerCount >= 2)
	{
		auto& Timer = GetWorldTimerManager();

		Timer.SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, StartDelay);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	PlayerCount--;
}

void ALobbyGameMode::StartGame()
{
	auto* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance());

	if (GameInstance != nullptr)
	{
		GameInstance->StartSession();
		
		bUseSeamlessTravel = true;

		if (GWorld != nullptr)
			GWorld->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
	}
}
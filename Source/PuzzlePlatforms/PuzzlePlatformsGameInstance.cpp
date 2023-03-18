// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> menuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (menuBPClass.Class != nullptr)
		MenuClass = menuBPClass.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> inGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (inGameMenuBPClass.Class != nullptr)
		InGameMenuClass = inGameMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, _T("Found class %s"), *MenuClass->GetName());
	UE_LOG(LogTemp, Warning, _T("Found class %s"), *InGameMenuClass->GetName());

	auto* oss = IOnlineSubsystem::Get();

	if (oss != nullptr)
	{
		UE_LOG(LogTemp, Warning, _T("OS : %s"), *oss->GetSubsystemName().ToString());

		SessionInterface = oss->GetSessionInterface();

		if (SessionInterface.IsValid() == true)
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);

			SessionSearch = MakeShareable(new FOnlineSessionSearch);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, _T("OS have nothing"));
	}
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	if (MenuClass != nullptr)
	{
		MainMenu = CreateWidget<UMainMenu>(this, MenuClass, _T("MenuClass"));

		if (MainMenu != nullptr)
			MainMenu->Activate(this);
	}
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (InGameMenuClass != nullptr)
		InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass, _T("InGameMenuClass"));
}

void UPuzzlePlatformsGameInstance::Host(const FText& LobbyName)
{
	if (SessionInterface.IsValid() == true)
	{
		auto* ExistingSession = SessionInterface->GetNamedSession(*LobbyName.ToString());

		if (ExistingSession != nullptr)
		{
			FOnDestroySessionCompleteDelegate destroyDelegate;

			destroyDelegate.BindLambda([this](FName sessionName, bool isSuccess)
				{
					if (isSuccess != true)
						return;

					UE_LOG(LogTemp, Warning, _T("Destory Session Complete"));

					CreateSession(sessionName);
				});

			SessionInterface->DestroySession(*LobbyName.ToString(), destroyDelegate);
		}
		else
		{
			CreateSession(*LobbyName.ToString());
		}
	}
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (SessionInterface.IsValid() != true || SessionSearch.IsValid() != true)
		return;

	SessionInterface->JoinSession(0, SETTING_GAME_SESSION_URI, SessionSearch->SearchResults[Index]);
}

void UPuzzlePlatformsGameInstance::StartSession()
{
	if (SessionInterface.IsValid() == true)
	{
		SessionInterface->StartSession(NAME_GameSession);
	}
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	if (SessionSearch.IsValid() == true)
	{
		//sessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::OpenInGameMenu()
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, _T("Open In Game Menu"));

		if (InGameMenu != nullptr)
			InGameMenu->Activate(this);
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

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bIsSuccess)
{
	if (bIsSuccess != true)
		return;

	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, _T("Hosting"));

		UE_LOG(LogTemp, Warning, _T("Create %s Session"), *SessionName.ToString());

		if (GWorld != nullptr)
			GWorld->ServerTravel("/Game/PuzzlePlatforms/Maps/Lobby?listen");
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bIsSuccess)
{

}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool bIsSuccess)
{
	if (bIsSuccess == true && SessionSearch.IsValid() == true)
	{
		UE_LOG(LogTemp, Warning, _T("Found Session"));

		if (MainMenu != nullptr)
		{
			MainMenu->ClearServerList();

			for (auto& result : SessionSearch->SearchResults)
			{
				FServerData data;

				data.maxPlayers = result.Session.SessionSettings.NumPublicConnections;
				data.currentPlayers = data.maxPlayers - result.Session.NumOpenPublicConnections;
				data.hostUserName = result.Session.OwningUserName;

				FString userData;

				if (result.Session.SessionSettings.Get<FString>(_T("LobbyName"), userData))
				{
					data.name = *userData;
				}

				MainMenu->AddServerRow(data);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, _T("Found not Session"));
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface.IsValid() == true)
	{
		FString connectInfo;

		if (SessionInterface->GetResolvedConnectString(SessionName, connectInfo) != true)
			return;

		UE_LOG(LogTemp, Warning, _T("%s"), *connectInfo);

		auto* playerController = GetFirstLocalPlayerController();

		if (playerController != nullptr)
			playerController->ClientTravel(connectInfo, ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzlePlatformsGameInstance::CreateSession(FName SessionName)
{
	if (SessionInterface.IsValid() == true)
	{
		auto* oss = IOnlineSubsystem::Get();

		FOnlineSessionSettings sessionSettings;

		sessionSettings.bIsLANMatch = oss->GetSubsystemName() == "NULL";
		sessionSettings.bUsesPresence = true;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.NumPublicConnections = 5;
		sessionSettings.bUseLobbiesIfAvailable = true;
		sessionSettings.Set(_T("LobbyName"), SessionName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		
		SessionInterface->CreateSession(0, NAME_GameSession, sessionSettings);
	}
}
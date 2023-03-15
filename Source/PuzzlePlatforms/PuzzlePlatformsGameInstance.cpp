// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/InGameMenu.h"

const static FName SESSION_NAME = _T("My Session Game");

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

	auto* oss = IOnlineSubsystem::Get();

	if (oss != nullptr)
	{
		UE_LOG(LogTemp, Warning, _T("OS : %s"), *oss->GetSubsystemName().ToString());

		sessionInterface = oss->GetSessionInterface();

		if (sessionInterface.IsValid() == true)
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);

			sessionSearch = MakeShareable(new FOnlineSessionSearch);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, _T("OS have nothing"));
	}
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
	if (sessionInterface.IsValid() == true)
	{
		auto* ExistingSession = sessionInterface->GetNamedSession(SESSION_NAME);

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

			sessionInterface->DestroySession(SESSION_NAME, destroyDelegate);
		}
		else
		{
			CreateSession(SESSION_NAME);
		}
	}
}

void UPuzzlePlatformsGameInstance::Join(uint32 index)
{
	if (sessionInterface.IsValid() != true || sessionSearch.IsValid() != true)
		return;

	sessionInterface->JoinSession(0, SESSION_NAME, sessionSearch->SearchResults[index]);
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	if (sessionSearch.IsValid() == true)
	{
		//sessionSearch->bIsLanQuery = true;
		sessionSearch->MaxSearchResults = 100;
		sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
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

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName sessionName, bool isSuccess)
{
	if (isSuccess != true)
		return;

	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, _T("Hosting"));

		UE_LOG(LogTemp, Warning, _T("Create %s Session"), *sessionName.ToString());

		if (GWorld != nullptr)
			GWorld->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName sessionName, bool isSuccess)
{

}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool isSuccess)
{
	if (isSuccess == true && sessionSearch.IsValid() == true)
	{
		UE_LOG(LogTemp, Warning, _T("Found Session"));

		if (mainMenu != nullptr)
		{
			mainMenu->ClearServerList();

			for (auto& result : sessionSearch->SearchResults)
			{
				FServerData data;

				data.name = result.GetSessionIdStr();
				data.currentPlayers = 0;
				data.maxPlayers = result.Session.NumOpenPublicConnections;
				data.hostUserName = result.Session.OwningUserName;

				mainMenu->AddServerRow(data);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, _T("Found not Session"));
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (sessionInterface.IsValid() == true)
	{
		FString connectInfo;

		if (sessionInterface->GetResolvedConnectString(sessionName, connectInfo) != true)
			return;

		UE_LOG(LogTemp, Warning, _T("%s"), *connectInfo);

		auto* playerController = GetFirstLocalPlayerController();

		if (playerController != nullptr)
			playerController->ClientTravel(connectInfo, ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzlePlatformsGameInstance::CreateSession(FName sessionName)
{
	if (sessionInterface.IsValid() == true)
	{
		auto* oss = IOnlineSubsystem::Get();

		FOnlineSessionSettings sessionSettings;

		sessionSettings.bIsLANMatch = oss->GetSubsystemName() == "NULL";
		sessionSettings.bUsesPresence = true;
		sessionSettings.bShouldAdvertise = true;
		sessionSettings.NumPublicConnections = 2;
		sessionSettings.bUseLobbiesIfAvailable = true;

		sessionInterface->CreateSession(0, sessionName, sessionSettings);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

private:
	TSubclassOf<class UMainMenu> menuClass;
	class UMainMenu* mainMenu;

	TSubclassOf<class UInGameMenu> inGameMenuClass;
	class UInGameMenu* inGameMenu;

	IOnlineSessionPtr sessionInterface;

	TSharedPtr<FOnlineSessionSearch> sessionSearch;

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	void Host(const FText& lobbyName) override;

	void Join(uint32 index) override;

	void RefreshServerList() override;

	UFUNCTION(BlueprintCallable)
	void OpenInGameMenu();

	UFUNCTION(Exec)
	void Leave() override;

	UFUNCTION(Exec)
	void QuitGame() override;

	void OnCreateSessionComplete(FName sessionName, bool isSuccess);
	void OnDestroySessionComplete(FName sessionName, bool isSuccess);
	void OnFindSessionsComplete(bool isSuccess);
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);

	void CreateSession(FName sessionName);
};
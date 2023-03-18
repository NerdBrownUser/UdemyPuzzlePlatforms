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
	TSubclassOf<class UMainMenu> MenuClass;

	UPROPERTY()
	class UMainMenu* MainMenu;

	TSubclassOf<class UInGameMenu> InGameMenuClass;

	UPROPERTY()
	class UInGameMenu* InGameMenu;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	virtual void Host(const FText& LobbyName) override;

	virtual void Join(uint32 Index) override;

	void StartSession();
	
	virtual void RefreshServerList() override;

	UFUNCTION(BlueprintCallable)
	void OpenInGameMenu();

	UFUNCTION()
	virtual void Leave() override;

	UFUNCTION()
	virtual void QuitGame() override;

	void OnCreateSessionComplete(FName SessionName, bool bIsSuccess);
	void OnDestroySessionComplete(FName SessionName, bool bIsSuccess);
	void OnFindSessionsComplete(bool bIsSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession(FName SessionName);
};
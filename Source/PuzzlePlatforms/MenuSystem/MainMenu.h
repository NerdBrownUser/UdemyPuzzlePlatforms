// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

private:
	class IMenuInterface* menuInterface;

	UPROPERTY(Meta = (BindWidget))
	class UWidgetSwitcher* menuSwitcher;

	/* Main Menu */
	UPROPERTY(Meta = (BindWidget))
	class UWidget* mainMenu;

	UPROPERTY(Meta = (BindWidget))
	class UButton* hostButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* joinMenuButton;

	/* Join Menu */
	UPROPERTY(Meta = (BindWidget))
	class UWidget* joinMenu;

	UPROPERTY(Meta = (BindWidget))
	class UEditableTextBox* inputIPAddress;

	UPROPERTY(Meta = (BindWidget))
	class UButton* joinButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* cancelJoinMenuButton;

	/* Quit Menu */
	UPROPERTY(Meta = (BindWidget))
	class UWidget* quitMenu;

	UPROPERTY(Meta = (BindWidget))
	class UButton* quitButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* cancelQuitMenuButton;

public:
	void Activate(class IMenuInterface* _menuInterface);
	void Deactivate();

	UFUNCTION(BlueprintCallable)
	void OpenQuitMenu();

protected:
	virtual bool Initialize() override;
	void NativeDestruct() override;
	
private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void QuitGame();
};
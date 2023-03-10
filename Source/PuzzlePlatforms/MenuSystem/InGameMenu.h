// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(Meta = (BindWidget))
	class UButton* leaveButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* cancelInGameMenuButton;

	class IMenuInterface* menuInterface;

public:
	void Activate(class IMenuInterface* _menuInterface);
	void Deactivate();

protected:
	virtual bool Initialize() override;
	void NativeDestruct() override;

private:
	UFUNCTION()
	void LeaveGame();

	UFUNCTION()
	void Close();
};
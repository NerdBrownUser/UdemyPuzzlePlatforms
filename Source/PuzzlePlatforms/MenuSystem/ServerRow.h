// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

private:
	class UMainMenu* parent;

	uint32 index;

	UPROPERTY(Meta = (BindWidget))
	class UButton* rowButton;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* playerCounts;

public:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* serverName;

	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* hostUserName;

public:
	void SetParentWidget(class UMainMenu* _parent, uint32 _index);

	void ChangeTextColor(float r, float g, float b, float a);

	void SetPlayerCounts(uint16 current, uint16 max);

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnClicked();
};
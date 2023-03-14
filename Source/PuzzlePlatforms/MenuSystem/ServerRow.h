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

public:
	UPROPERTY(Meta = (BindWidget))
	class UTextBlock* serverName;

public:
	void SetParentWidget(class UMainMenu* _parent, uint32 _index);

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnClicked();
};
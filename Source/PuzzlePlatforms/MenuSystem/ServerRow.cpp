// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UServerRow::SetParentWidget(UMainMenu* _parent, uint32 _index)
{
	parent = _parent;

	index = _index;
}

void UServerRow::ChangeTextColor(float r, float g, float b, float a)
{
	if (serverName != nullptr)
	{
		FLinearColor color(r, g, b, a);

		serverName->SetColorAndOpacity(color);
		hostUserName->SetColorAndOpacity(color);
		playerCounts->SetColorAndOpacity(color);
	}
}

void UServerRow::SetPlayerCounts(uint16 current, uint16 max)
{
	if (playerCounts != nullptr)
	{
		playerCounts->SetText(FText::FromString(FString::Printf(_T("%d/%d"), current, max)));
	};
}

bool UServerRow::Initialize()
{
	if (Super::Initialize() == false ||
		rowButton == nullptr)
		return false;

	rowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);

	return true;
}

void UServerRow::OnClicked()
{
	UE_LOG(LogTemp, Warning, _T("Row Button Clicked"));

	if (parent != nullptr)
		parent->SelectIndex(index);
}
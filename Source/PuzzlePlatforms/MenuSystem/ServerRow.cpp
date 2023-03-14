// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"

#include "MainMenu.h"

#include "Components/Button.h"

void UServerRow::SetParentWidget(UMainMenu* _parent, uint32 _index)
{
	parent = _parent;

	index = _index;
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
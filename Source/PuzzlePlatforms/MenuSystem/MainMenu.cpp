// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	if (Super::Initialize() == false || host == nullptr)
		return false;

	host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return false;
}

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, _T("Server hosting"));
}
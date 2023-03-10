// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "MenuInterface.h"
#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	if (Super::Initialize() == false || host == nullptr)
		return false;

	host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	menuInterface = GetGameInstance<IMenuInterface>();

	return true;
}

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Warning, _T("Server hosting"));

	if (menuInterface != nullptr)
	{
		menuInterface->Host();
	}
}
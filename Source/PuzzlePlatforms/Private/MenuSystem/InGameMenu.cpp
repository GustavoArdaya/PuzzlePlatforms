// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/InGameMenu.h"

#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize() || !CancelButton || !MainMenuButton) return false;

	CancelButton->OnClicked.AddDynamic(this, &ThisClass::Cancel);
	MainMenuButton->OnClicked.AddDynamic(this, &ThisClass::GoToMainMenu);
	
	return true;
}

void UInGameMenu::Cancel()
{
	Teardown();
}

void UInGameMenu::GoToMainMenu()
{
	if (MenuInterface)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}

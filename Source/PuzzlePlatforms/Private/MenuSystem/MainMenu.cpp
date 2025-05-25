// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/MainMenu.h"

#include "Components/Button.h"

bool UMainMenu::Initialize()
{
	if (!Super::Initialize() || !HostButton || !JoinButton) return false;

	HostButton->OnClicked.AddDynamic(this, &ThisClass::HostServer);
	
	return true;
}

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Display, TEXT("Host Button Clicked"));
}

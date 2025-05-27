// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
	SetIsFocusable(true);
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize() || !HostButton || !JoinMenuButton) return false;

	JoinMenuButton->OnClicked.AddDynamic(this, &ThisClass::OpenJoinMenu);
	BackButton->OnClicked.AddDynamic(this, &ThisClass::OpenMainMenu);
	HostButton->OnClicked.AddDynamic(this, &ThisClass::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinServer);
	
	return true;
}

void UMainMenu::HostServer()
{
	UE_LOG(LogTemp, Display, TEXT("Host Button Clicked"));
	if (MenuInterface)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface && IPAddressField)
	{
		const FString& Address = IPAddressField->GetText().ToString();
		const FString& HintText = IPAddressField->GetHintText().ToString();
		if (!Address.Equals(HintText))
		{
			MenuInterface->Join(IPAddressField->GetText().ToString());			
		}
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher && JoinMenu)
	{
		MenuSwitcher->SetActiveWidget(JoinMenu);
	}
}

void UMainMenu::OpenMainMenu()
{
	if (MenuSwitcher && MainMenu)
	{
		MenuSwitcher->SetActiveWidget(MainMenu);
	}
}

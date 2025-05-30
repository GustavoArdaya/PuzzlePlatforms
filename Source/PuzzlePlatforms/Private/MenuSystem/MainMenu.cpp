// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MenuSystem/ServerRow.h"

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
	QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::QuitGame);
	
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

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	ServerList->ClearChildren();	
	for (const FString& ServerName : ServerNames)
	{
		if (UClass* ServerRowLoadedClass = ServerRowClass.LoadSynchronous())
		{
			Row = CreateWidget<UServerRow>(this, ServerRowLoadedClass);
			if (Row)
			{
				Row->ServerName->SetText(FText::FromString(ServerName));
				ServerList->AddChild(Row);
			}
		}
	}	
}

void UMainMenu::JoinServer()
{
	if (MenuInterface && ServerList)
	{		
		MenuInterface->Join("");					
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher && JoinMenu)
	{
		MenuSwitcher->SetActiveWidget(JoinMenu);
		if (MenuInterface)
		{
			MenuInterface->RefreshServerList();
		}
	}
}

void UMainMenu::OpenMainMenu()
{
	if (MenuSwitcher && MainMenu)
	{
		MenuSwitcher->SetActiveWidget(MainMenu);
	}
}

void UMainMenu::QuitGame()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}

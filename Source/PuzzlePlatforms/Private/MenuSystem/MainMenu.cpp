// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/MainMenu.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
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

void UMainMenu::JoinServer()
{
	if (MenuInterface && ServerList)
	{
		if (UClass* ServerRowLoadedClass = ServerRowClass.LoadSynchronous())
		{
			ServerRowWidget = CreateWidget<UServerRow>(this, ServerRowLoadedClass);
			if (ServerRowWidget)
			{
				ServerList->AddChild(ServerRowWidget);
			}
		}
		/*const FString& Address = ServerList->GetText().ToString();
		const FString& HintText = ServerList->GetHintText().ToString();
		if (!Address.Equals(HintText))
		{
			MenuInterface->Join(ServerList->GetText().ToString());			
		}*/
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

void UMainMenu::QuitGame()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/MainMenu.h"

#include "VisualizeTexture.h"
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
	uint32 Index = 0;
	for (const FString& ServerName : ServerNames)
	{
		if (UClass* ServerRowLoadedClass = ServerRowClass.LoadSynchronous())
		{
			Row = CreateWidget<UServerRow>(this, ServerRowLoadedClass);
			if (Row)
			{
				Row->ServerName->SetText(FText::FromString(ServerName));
				Row->Setup(this, Index);
				++Index;
				ServerList->AddChild(Row);
			}
		}
	}	
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); i++)
	{
		if (auto ServerRow = Cast<UServerRow>(ServerList->GetChildAt(i)))
		{
			ServerRow->bIsSelected = SelectedIndex.IsSet() && SelectedIndex.GetValue() == i;
		}
	}
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No selected index"));
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



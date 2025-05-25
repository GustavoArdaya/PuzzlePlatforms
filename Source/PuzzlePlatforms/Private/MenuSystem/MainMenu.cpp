// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/MainMenu.h"

#include "Components/Button.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{	
	SetIsFocusable(true);
}

void UMainMenu::Setup()
{
	this->AddToViewport();
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(this->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			
		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = true;		
	}
}

void UMainMenu::Teardown()
{
	this->RemoveFromParent();
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameOnly InputModeData;

		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;		
	};
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize() || !HostButton || !JoinButton) return false;

	HostButton->OnClicked.AddDynamic(this, &ThisClass::HostServer);
	
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

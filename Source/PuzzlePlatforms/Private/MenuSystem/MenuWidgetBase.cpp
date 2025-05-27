// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/MenuWidgetBase.h"

void UMenuWidgetBase::Setup()
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

void UMenuWidgetBase::Teardown()
{
	this->RemoveFromParent();
	
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameOnly InputModeData;

		PlayerController->SetInputMode(InputModeData);
		PlayerController->bShowMouseCursor = false;		
	};
}

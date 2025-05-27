// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidgetBase.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{	
	UE_LOG(LogTemp, Warning, TEXT("GameInstance constructed"));
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogTemp, Warning, TEXT("GameInstance initialized"));
	if (MenuWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuWidgetClass->GetName());
	}
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (MenuWidgetClass.IsValid() || !MenuWidgetClass.IsNull())
	{
		UClass* LoadedMainMenuClass = MenuWidgetClass.LoadSynchronous();
		MainMenuWidget = CreateWidget<UMenuWidgetBase>(this, LoadedMainMenuClass);

		if (MainMenuWidget)
		{
			MainMenuWidget->Setup();
			MainMenuWidget->SetMenuInterface(this);
		}
	}
}

void UPuzzlePlatformsGameInstance::LoadInGameMenu()
{
	if (InGameMenuClass.IsValid() || !InGameMenuClass.IsNull())
	{
		UClass* LoadedInGameMenuClass = InGameMenuClass.LoadSynchronous();
		InGameMenuWidget = CreateWidget<UMenuWidgetBase>(this, LoadedInGameMenuClass);

		if (InGameMenuWidget)
		{
			InGameMenuWidget->Setup();
			InGameMenuWidget->SetMenuInterface(this);
		}
	}
}

void UPuzzlePlatformsGameInstance::Host()
{
	
	const FString MapName = "/Game/ThirdPerson/Maps/ThirdPersonMap";
	
	if (MainMenuWidget)
	{
		MainMenuWidget->Teardown();
	}
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Traveling to %s"), *MapName));
	}

	if (UWorld* World = GetWorld())
	{
		World->ServerTravel(MapName + "?listen");
	}
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	if (MainMenuWidget)
    {
    	MainMenuWidget->Teardown();
    }

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	}

	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

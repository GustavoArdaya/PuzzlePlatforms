// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"

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
	if (MenuWidgetClass)
	{
		if (UUserWidget* MenuWidget = CreateWidget<UUserWidget>(this, MenuWidgetClass.Get()))
		{
			MenuWidget->AddToViewport();
		}
	}
}

void UPuzzlePlatformsGameInstance::Host()
{
	const FString MapName = "/Game/ThirdPerson/Maps/ThirdPersonMap";
	
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	}

	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}
}

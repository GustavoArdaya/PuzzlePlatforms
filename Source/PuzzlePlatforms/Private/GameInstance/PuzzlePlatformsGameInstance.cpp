// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "MenuSystem/MenuWidgetBase.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("My Session Game");


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{	
	UE_LOG(LogTemp, Warning, TEXT("GameInstance constructed"));
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{		
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem Subsystem Name: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{			
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDestroySessionComplete);
			
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionComplete);			
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Starting FindSessions"));
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());				
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no Subsystem"));
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
	if (SessionInterface.IsValid())
	{		
		if (FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME))
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}		
	}	
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}
	
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

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Found session"));
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);	
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

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		if (MainMenuMap.ToSoftObjectPath().IsValid())
		{
			FString MapPath = MainMenuMap.ToSoftObjectPath().ToString();
			// Trim any "/Game/Maps/MyMap.MyMap" to "/Game/Maps/MyMap"
			MapPath = FPackageName::ObjectPathToPackageName(MapPath);
			
			PlayerController->ClientTravel(MapPath, ETravelType::TRAVEL_Absolute);
		}
	}
}



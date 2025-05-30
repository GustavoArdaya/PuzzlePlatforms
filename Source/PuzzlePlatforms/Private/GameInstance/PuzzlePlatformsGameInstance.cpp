// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "SocketSubsystem.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MainMenu.h"

const static FName SESSION_NAME = TEXT("My Session Game");


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{	
	UE_LOG(LogTemp, Warning, TEXT("GameInstance constructed"));
}

void UPuzzlePlatformsGameInstance::Init()
{
	Super::Init();
	// DebugNetworkEnvironment();

	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{		
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem Subsystem Name: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{			
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDestroySessionComplete);
			
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionsComplete);			
			
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
		MainMenuWidget = CreateWidget<UMainMenu>(this, LoadedMainMenuClass);

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
		InGameMenuWidget = CreateWidget<UInGameMenu>(this, LoadedInGameMenuClass);

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

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful && SessionSearch.IsValid() && MainMenuWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found sessions: %d"), SessionSearch->SearchResults.Num());
		TArray<FString> ServerNames; 
		for (auto SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Session named: %s"), *SearchResult.GetSessionIdStr());
			ServerNames.Add(*SearchResult.GetSessionIdStr());
		}
		MainMenuWidget->SetServerList(ServerNames);
	}	
}

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);	
	}		
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	if (MainMenuWidget)
    {
    	//MainMenuWidget->Teardown();
		MainMenuWidget->SetServerList({"Test1", "Test2", "Test3", "Test4", "Test5"});
    }

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));
	}

	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
	}*/
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

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		// SessionSearch->bIsLanQuery = true;
		UE_LOG(LogTemp, Warning, TEXT("Starting FindSessions"));
				
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());				
	}
}

void UPuzzlePlatformsGameInstance::DebugNetworkEnvironment()
{
	UE_LOG(LogTemp, Warning, TEXT("----- BEGIN NETWORK DEBUG -----"));

	// Online Subsystem
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("OnlineSubsystem not found!"));
	}
	else
	{
		FString SubsystemName = Subsystem->GetSubsystemName().ToString();
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem: %s"), *SubsystemName);

		if (SubsystemName == "NULL")
		{
			UE_LOG(LogTemp, Warning, TEXT("Using NULL subsystem — only LAN is supported."));
		}

		SessionInterface = Subsystem->GetSessionInterface();
		if (!SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("SessionInterface is invalid!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SessionInterface is valid."));
		}
	}

	// Local IP Address
	bool bCanBind = false;
	TSharedRef<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);
	if (LocalAddr->IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Local IP Address: %s"), *LocalAddr->ToString(false));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not retrieve local IP address."));
	}

	// Network Interfaces (fixed for UE 5.5)
	UE_LOG(LogTemp, Warning, TEXT("Network Interfaces:"));
	TArray<TSharedRef<FInternetAddr>> Addresses = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalBindAddresses();

	for (const TSharedRef<FInternetAddr>& Addr : Addresses)
	{
		UE_LOG(LogTemp, Warning, TEXT(" - %s"), *Addr->ToString(true));
	}

	// Port check reminder
	UE_LOG(LogTemp, Warning, TEXT("Default port for listen server: 7777 (verify it's open on the host)."));

	// Editor vs Standalone
	if (GIsEditor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Running inside the Editor (PIE or Simulate)."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Running in Standalone or Packaged build."));
	}

	// Dedicated server?
	if (IsRunningDedicatedServer())
	{
		UE_LOG(LogTemp, Warning, TEXT("Running as a Dedicated Server."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not a Dedicated Server."));
	}

	// NetMode
	if (UWorld* World = GetWorld())
	{
		switch (World->GetNetMode())
		{
		case NM_Standalone:
			UE_LOG(LogTemp, Warning, TEXT("NetMode: Standalone"));
			break;
		case NM_Client:
			UE_LOG(LogTemp, Warning, TEXT("NetMode: Client"));
			break;
		case NM_ListenServer:
			UE_LOG(LogTemp, Warning, TEXT("NetMode: Listen Server"));
			break;
		case NM_DedicatedServer:
			UE_LOG(LogTemp, Warning, TEXT("NetMode: Dedicated Server"));
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("NetMode: Unknown"));
			break;
		}
	}

	// Manual network advice
	UE_LOG(LogTemp, Warning, TEXT("Make sure no firewall or antivirus is blocking UE4Editor or UE5Editor."));
	UE_LOG(LogTemp, Warning, TEXT("LAN discovery uses UDP broadcast — some routers may block this."));
	UE_LOG(LogTemp, Warning, TEXT("If running via VPN or virtual adapters (like Hamachi), LAN might fail."));

	UE_LOG(LogTemp, Warning, TEXT("Use 'netstat -a -n | find \"7777\"' to verify port binding on host."));
	UE_LOG(LogTemp, Warning, TEXT("----- END NETWORK DEBUG -----"));
}

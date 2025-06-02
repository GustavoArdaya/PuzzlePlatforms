#include "GameInstance/PuzzlePlatformsGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "SocketSubsystem.h"
#include "MenuSystem/InGameMenu.h"
#include "MenuSystem/MainMenu.h"
#include "Online/OnlineSessionNames.h"

const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SERVER_NAME_KEY = TEXT("ServerName");
const static FName GAME_TAG_KEY = TEXT("GameTag");
const static FString GAME_TAG_VALUE = TEXT("PuzzlePlatforms");

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
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::OnJoinSessionComplete);
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

void UPuzzlePlatformsGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings = CreateDefaultSessionSettings();
		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

FOnlineSessionSettings UPuzzlePlatformsGameInstance::CreateDefaultSessionSettings()
{
	FOnlineSessionSettings SessionSettings;

	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;

	SessionSettings.Set(GAME_TAG_KEY, GAME_TAG_VALUE, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//SessionSettings.Set(SERVER_NAME_KEY, FString("Gus's PuzzlePlatforms"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	return SessionSettings;
}

TSharedPtr<FOnlineSessionSearch> UPuzzlePlatformsGameInstance::CreateDefaultSessionSearch()
{
	TSharedPtr<FOnlineSessionSearch> Search = MakeShareable(new FOnlineSessionSearch());
	Search->MaxSearchResults = 10000;

	Search->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	Search->QuerySettings.Set(GAME_TAG_KEY, GAME_TAG_VALUE, EOnlineComparisonOp::Equals);

	return Search;
}

void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	SessionSearch = CreateDefaultSessionSearch();
	if (SessionSearch.IsValid() && SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Starting FindSessions with filters"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (!SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionSearch is invalid in OnFindSessionsComplete"));
		return;
	}

	TArray<FServerData> ServerList;

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %d sessions"), SessionSearch->SearchResults.Num());

		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			FServerData ServerData;

			ServerData.ServerName = Result.GetSessionIdStr();
			ServerData.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			ServerData.CurrentPlayers = Result.Session.NumOpenPublicConnections;

			FString HostName;
			if (Result.Session.OwningUserName.IsEmpty())
			{
				HostName = TEXT("Unknown Host");
			}
			else
			{
				HostName = Result.Session.OwningUserName;
			}
			ServerData.HostUserName = HostName;

			ServerList.Add(ServerData);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSessions failed."));
	}

	if (MainMenuWidget)
	{
		MainMenuWidget->SetServerList(ServerList); // Make sure SetServerList accepts TArray<FServerData>
	}
}

void UPuzzlePlatformsGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Join failed: SessionInterface or SessionSearch is invalid."));
		return;
	}

	if (Index >= static_cast<uint32>(SessionSearch->SearchResults.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Join failed: Index %d out of range (Num: %d)."), Index, SessionSearch->SearchResults.Num());
		return;
	}

	if (MainMenuWidget)
	{
		MainMenuWidget->Teardown();
	}

	// Make a copy of the search result so we can modify the settings
	FOnlineSessionSearchResult SearchResult = SessionSearch->SearchResults[Index];
	if (!SearchResult.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Join failed: SearchResult at index %d is invalid."), Index);
		return;
	}

	// Patch the settings flags to match (workaround for Steam UE5.5 bug)
	SearchResult.Session.SessionSettings.bUseLobbiesIfAvailable = SearchResult.Session.SessionSettings.bUsesPresence;

	UE_LOG(LogTemp, Warning, TEXT("Joining session at index %d with bUseLobbiesIfAvailable = %s and bUsesPresence = %s"),
		Index,
		SearchResult.Session.SessionSettings.bUseLobbiesIfAvailable ? TEXT("true") : TEXT("false"),
		SearchResult.Session.SessionSettings.bUsesPresence ? TEXT("true") : TEXT("false"));

	bool bJoinStarted = SessionInterface->JoinSession(0, SESSION_NAME, SearchResult);
	if (!bJoinStarted)
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSession call failed for session at index %d."), Index);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("JoinSession call started for session at index %d."), Index);
	}
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	if (MainMenuWidget)
	{
		MainMenuWidget->Teardown();
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
	}

	if (UWorld* World = GetWorld())
	{
		World->ServerTravel(TEXT("/Game/ThirdPerson/Maps/ThirdPersonMap?listen"));
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		CreateSession();
	}
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session"));
		return;
	}

	FString ConnectInfo;
	if (!SessionInterface->GetResolvedConnectString(SessionName, ConnectInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}

	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		PlayerController->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
	}
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	if (APlayerController* PlayerController = GetFirstLocalPlayerController())
	{
		if (MainMenuMap.ToSoftObjectPath().IsValid())
		{
			FString MapPath = FPackageName::ObjectPathToPackageName(MainMenuMap.ToSoftObjectPath().ToString());
			PlayerController->ClientTravel(MapPath, ETravelType::TRAVEL_Absolute);
		}
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

	// Network Interfaces (UE 5.5-compatible)
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



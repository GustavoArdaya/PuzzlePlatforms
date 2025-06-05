// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"

#include "OnlineSubsystem.h"
#include "GameInstance/PuzzlePlatformsGameInstance.h"
#include "OnlineSessionSettings.h"
#include "PuzzlePlatformsSessionConstants.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerCount++;
	UpdatePlayerCountInMetadata();

	if (PlayerCount >= 3)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Number of players: %d"), PlayerCount);
		if (UWorld* World = GetWorld();
			const UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(GetGameInstance()))
		{
			const FString GameMapPath = FPackageName::ObjectPathToPackageName(GameInstance->GameMap.ToSoftObjectPath().ToString()) + "?listen";
			bUseSeamlessTravel = true;
			World->ServerTravel(GameMapPath);
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PlayerCount--;
	UpdatePlayerCountInMetadata();
}

void ALobbyGameMode::UpdatePlayerCountInMetadata()
{
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FNamedOnlineSession* Session = SessionInterface->GetNamedSession(SESSION_NAME);
			if (Session)
			{
				Session->SessionSettings.Set(FName("CURRENT_PLAYERS"), PlayerCount, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
				SessionInterface->UpdateSession(Session->SessionName, Session->SessionSettings, true);
			}
		}
	}
}

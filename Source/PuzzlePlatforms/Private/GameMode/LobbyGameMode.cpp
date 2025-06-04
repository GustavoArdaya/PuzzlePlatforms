// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"

#include "GameInstance/PuzzlePlatformsGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PlayerCount++;
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
}

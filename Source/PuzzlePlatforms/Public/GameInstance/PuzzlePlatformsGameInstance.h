// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class UInGameMenu;
class UMenuWidgetBase;
class UMainMenu;
class FOnlineSessionSearch;
/**
 * 
 */
UCLASS()
class UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UMainMenu> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UInGameMenu> InGameMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maps")
	TSoftObjectPtr<UWorld> MainMenuMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Maps")
	TSoftObjectPtr<UWorld> LobbyMap;

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Exec)
	void LoadMenu();

	UFUNCTION(BlueprintCallable, Exec)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	virtual void Host(FString ServerName) override;
	
	UFUNCTION(Exec)
	virtual void Join(uint32 Index) override;

	UFUNCTION(Exec)
	virtual void LoadMainMenu() override;

	UFUNCTION(Exec)
	virtual void RefreshServerList() override;

	UFUNCTION(BlueprintCallable, Exec)
	void DebugNetworkEnvironment();

private:

	UMainMenu* MainMenuWidget;
	UInGameMenu* InGameMenuWidget;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FString DesiredServerName;
	void CreateSession();
	
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// New
	FOnlineSessionSettings CreateDefaultSessionSettings();
	TSharedPtr<FOnlineSessionSearch> CreateDefaultSessionSearch();
};

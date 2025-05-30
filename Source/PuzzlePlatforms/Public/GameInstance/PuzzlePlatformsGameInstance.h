// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

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
	TSoftClassPtr<UUserWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> InGameMenuClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main Menu")
	TSoftObjectPtr<UWorld> MainMenuMap;

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Exec)
	void LoadMenu();

	UFUNCTION(BlueprintCallable, Exec)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	virtual void Host() override;
	
	UFUNCTION(Exec)
	virtual void Join(const FString& Address) override;

	UFUNCTION(Exec)
	virtual void LoadMainMenu() override;

private:

	UMenuWidgetBase* MainMenuWidget;
	UMenuWidgetBase* InGameMenuWidget;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void CreateSession();
	
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};

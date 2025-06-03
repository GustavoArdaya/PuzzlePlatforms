// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString ServerName;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUserName;
};

class UServerRow;
/**
 * 
 */
UCLASS()
class UMainMenu : public UMenuWidgetBase
{
	GENERATED_BODY()

public:

	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

protected:
	
	virtual bool Initialize() override;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> ServerRowClass;
	
private:

	UPROPERTY(Meta = (BindWidget))
	class UButton* HostMenuButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* BackButtonHost;

	UPROPERTY(Meta = (BindWidget))
	class UButton* JoinMenuButton;	

	UPROPERTY(Meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* BackButtonJoin;

	UPROPERTY(Meta = (BindWidget))
	class UButton* QuitGameButton;

	UPROPERTY(Meta = (BindWidget))
	class UEditableText* ServerHostName;

	UPROPERTY(Meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(Meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(Meta = (BindWidget))
	class UWidget* HostMenu;
	
	UPROPERTY(Meta = (BindWidget))
	class UWidget* JoinMenu;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenHostMenu();
	
	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void QuitGame();

	UServerRow* Row;

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

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

protected:
	
	virtual bool Initialize() override;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> ServerRowClass;
	
private:

	UPROPERTY(Meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* JoinMenuButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* BackButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* QuitGameButton;

	UPROPERTY(Meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(Meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(Meta = (BindWidget))
	class UWidget* JoinMenu;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void QuitGame();

	UServerRow* ServerRowWidget;
	
};

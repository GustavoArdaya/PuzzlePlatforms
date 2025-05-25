// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UMainMenu(const FObjectInitializer& ObjectInitializer);
	

	void Setup();
	void Teardown(); 
	FORCEINLINE void SetMenuInterface(IMenuInterface* InMenuInterface) { MenuInterface = InMenuInterface; }

protected:
	
	virtual bool Initialize() override;
	
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
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(Meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(Meta = (BindWidget))
	class UWidget* JoinMenu;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	IMenuInterface* MenuInterface;
	
};

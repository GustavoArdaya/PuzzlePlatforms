// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UInGameMenu : public UMenuWidgetBase
{
	GENERATED_BODY()

public:

protected:

	virtual bool Initialize() override;

private:

	UPROPERTY(Meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* MainMenuButton;

	UFUNCTION()
	void Cancel();
	
	UFUNCTION()
	void GoToMainMenu();
	
};

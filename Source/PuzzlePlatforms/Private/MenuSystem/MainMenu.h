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

	FORCEINLINE void SetMenuInterface(IMenuInterface* InMenuInterface) { MenuInterface = InMenuInterface; }

protected:
	
	virtual bool Initialize() override;
	
private:

	UPROPERTY(Meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(Meta = (BindWidget))
	class UButton* JoinButton;

	UFUNCTION()
	void HostServer();

	IMenuInterface* MenuInterface;
	
};

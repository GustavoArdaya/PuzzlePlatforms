// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"

class UMainMenu;
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

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Exec)
	void LoadMenu();

	UFUNCTION(Exec)
	void Host();
	
	UFUNCTION(Exec)
	void Join(const FString& Address);

private:

	UMainMenu* MenuWidget;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UPuzzlePlatformsGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UUserWidget> MenuWidgetClass;

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(Exec)
	void Host();
	
	UFUNCTION(Exec)
	void Join(const FString& Address);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

class UButton;
class UTextBlock;
class UMainMenu;
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(Meta = (BindWidget))
	UTextBlock* ServerName;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSelected = false;
	
	void Setup(UMainMenu* InParent, uint32 InIndex);

private:
	
	UPROPERTY(Meta = (BindWidget))
	UButton* RowButton;

	UMainMenu* Parent;

	uint32 Index;

	UFUNCTION()
	void OnClicked();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	
	AMovingPlatform();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Movement)
	float MovementSpeed = 20.f;

	UPROPERTY(EditAnywhere, Category = Movement, meta=(MakeEditWidget = true))
	FVector TargetLocation;


private:

	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;

	
	
};

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
	
	virtual void Tick(float DeltaTime) override;


private:

	UPROPERTY(EditAnywhere, Category = Movement)
	float MovementSpeed = 10.f;
};

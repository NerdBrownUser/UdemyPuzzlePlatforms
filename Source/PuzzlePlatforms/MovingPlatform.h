// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
private:
	FTransform originalTransform;

	bool isMoveToTarget = true;

	float remainToBegin;

public:
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector targetLocation;

	UPROPERTY(EditAnywhere)
	float moveSpeed = 100.0f;

	UPROPERTY(EditAnywhere)
	float waitTime = 3.0f;

	UPROPERTY(EditAnywhere)
	int needToActive = 0;

public:
	AMovingPlatform();

	void BeginPlay() override;

	void Tick(float deltaTime) override;

	bool Wait(float deltaTime);

	void Move(float deltaTime);
};
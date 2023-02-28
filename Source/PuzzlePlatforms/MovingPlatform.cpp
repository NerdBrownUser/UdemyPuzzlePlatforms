// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == true)
	{
		originalTransform = GetTransform();

		remainToBegin = waitTime;

		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (HasAuthority() == true)
	{
		if (Wait(deltaTime) == true)
			Move(deltaTime);
	}
}

bool AMovingPlatform::Wait(float deltaTime)
{
	if (remainToBegin <= 0)
		return true;

	remainToBegin -= deltaTime;

	return false;
}

void AMovingPlatform::Move(float deltaTime)
{
	FVector location = GetActorLocation();
	FVector destination;

	if (isMoveToTarget == true)
		destination = originalTransform.TransformPosition(targetLocation);
	else
		destination = originalTransform.GetLocation();

	float distance = (destination - location).Length();

	FVector direction = (destination - location).GetSafeNormal();

	if (distance <= moveSpeed * deltaTime)
	{
		if (deltaTime <= waitTime)
		{
			location = destination;

			remainToBegin = waitTime - deltaTime;
		}
		else
		{
			location -= direction * (moveSpeed * deltaTime - distance * 2);
		}

		isMoveToTarget = !isMoveToTarget;
	}
	else
	{
		location += direction * moveSpeed * deltaTime;
	}

	SetActorLocation(location);
}
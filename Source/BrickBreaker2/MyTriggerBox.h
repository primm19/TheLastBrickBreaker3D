// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MyTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class BRICKBREAKER2_API AMyTriggerBox : public ATriggerBox
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	AMyTriggerBox();

	UFUNCTION()
	void Overlapped(class AActor* overlappedActor, class AActor* otherActor);

	class ABossBall* BossBall;
	class ABall* ball;
	class ABrickBreakerGameMode* BrickBreakerGameMode;

	TArray<AActor*> FoundBalls;
};

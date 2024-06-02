// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BrickBreakerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BRICKBREAKER2_API ABrickBreakerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABrickBreakerGameMode();

	UPROPERTY(EditAnywhere)
	int32 NumOfBricks;
	
	UFUNCTION()
	void LevelChanger();

	TArray<FString> levels;

	FString CurrentLevel;
	int32 CurrentLevelIndex;

	FTimerHandle SpawnTimer;
	FTimerHandle CheckTimer;

	class APowerUpBrick* PowerUpBrick;

	void ReplaceWithPowerUpBrick();

	TArray<AActor*> FoundBricks;

	UPROPERTY(EditAnywhere, Category = "Balls")
	TSubclassOf<class ABall> BallTSubClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private: 
	UPROPERTY(EditAnywhere, Category = "PowerUps")
    TSubclassOf<class APowerUpBrick> PowerUpBrickClass;

	UPROPERTY(EditAnywhere)
	int32 NumOfPowerBricks;

	UPROPERTY(EditAnywhere)
	int32 NumOfBossBricks;
};

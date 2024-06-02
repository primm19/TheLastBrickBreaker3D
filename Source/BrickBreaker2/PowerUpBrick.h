// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBrick.h"
#include "PowerUpBrick.generated.h"

/**
 * 
 */
UCLASS()
class BRICKBREAKER2_API APowerUpBrick : public AEnemyBrick
{
	GENERATED_BODY()
	
public:
    APowerUpBrick();

	FTimerHandle DespawnTimer;
	FTimerHandle NormalizeBallSpeedTimer;

	void Despawn();

	void InitializePowerUp(AEnemyBrick* InOriginalBrick);

	void HandlePowerUpHit();

	UPROPERTY(EditAnywhere, Category = "PowerUps")
    TSubclassOf<class ABall> BallClass;

	int32 RandomFunctionInt;

	void CallElongatePaddle();
	void CallMultiBall();
	void CallSpeed();

	void NormalizeBallSpeed();
protected:
    virtual void BeginPlay() override;

private:
	AEnemyBrick* OriginalBrick;

	class ABrickBreakerGameMode* GameMode;

	class APlayerPaddle* PlayerPaddle;

	class ABall* Ball;
};
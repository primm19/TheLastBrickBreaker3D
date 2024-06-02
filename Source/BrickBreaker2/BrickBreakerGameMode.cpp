// Fill out your copyright notice in the Description page of Project Settings.


#include "BrickBreakerGameMode.h"
#include "Ball.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBrick.h"
#include "PowerUpBrick.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "PlayerPaddle.h"
#include "BossBrick.h"

ABrickBreakerGameMode::ABrickBreakerGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    
    levels.Add("MainMenu");
    levels.Add("Level1");
    levels.Add("Level2");
    levels.Add("Level3");
    levels.Add("Level4");
    levels.Add("Level5");
    levels.Add("Level6");
    levels.Add("BossLevel");
}

void ABrickBreakerGameMode::BeginPlay()
{
    Super::BeginPlay();

	FoundBricks.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBrick::StaticClass(), FoundBricks);

	NumOfBricks = FoundBricks.Num();

    CurrentLevel = GetWorld()->GetMapName();
    CurrentLevel.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
    CurrentLevelIndex = levels.IndexOfByKey(CurrentLevel);

    PowerUpBrick = Cast<class APowerUpBrick>(UGameplayStatics::GetActorOfClass(this, APowerUpBrick::StaticClass()));

    GetWorldTimerManager().SetTimer(SpawnTimer, this, &ABrickBreakerGameMode::ReplaceWithPowerUpBrick, 5.f);

}

void ABrickBreakerGameMode::LevelChanger()
{
    if(NumOfBricks <= 0)
    {
        APlayerPaddle* PlayerPaddle = Cast<class APlayerPaddle>(UGameplayStatics::GetActorOfClass(this, APlayerPaddle::StaticClass()));
        PlayerPaddle->Shrink();

        if (PowerUpBrick)
        {
            PowerUpBrick->GetWorldTimerManager().ClearTimer(PowerUpBrick->NormalizeBallSpeedTimer);
        }

        if (CurrentLevelIndex != -1)
        {
            UGameplayStatics::OpenLevel(GetWorld(), FName(levels[CurrentLevelIndex + 1]));
        } else {
            // Handle the case where the current level is not found in the array
            UE_LOG(LogTemp, Error, TEXT("Current Level not found in the array, index may have gone out of bounds or some other error may have occurred with ABrickBreakerGameMode::LevelChanger()"));
        }
    }
}

void ABrickBreakerGameMode::ReplaceWithPowerUpBrick()
{
    TArray<AActor*> BrickActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBrick::StaticClass(), BrickActors);

    if (BrickActors.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, BrickActors.Num() - 1);
        AEnemyBrick* OriginalBrick = CastChecked<AEnemyBrick>(BrickActors[RandomIndex]);

        if (OriginalBrick && !PowerUpBrick)
        {
            FVector Location = OriginalBrick->GetActorLocation();
            FRotator Rotation = OriginalBrick->GetActorRotation();

            PowerUpBrick = GetWorld()->SpawnActor<APowerUpBrick>(PowerUpBrickClass, Location, Rotation);

            if (PowerUpBrick)
            {
                // Copy the orbit properties from the brick to be replaced to the power-up brick
                PowerUpBrick->OrbitCenter = OriginalBrick->OrbitCenter;
                PowerUpBrick->OrbitRadius = OriginalBrick->OrbitRadius;
                PowerUpBrick->OrbitSpeed = OriginalBrick->OrbitSpeed;
                PowerUpBrick->bShouldMove = OriginalBrick->bShouldMove;
                PowerUpBrick->InitializePowerUp(OriginalBrick);
                OriginalBrick->SetActorHiddenInGame(true);
                OriginalBrick->SetActorEnableCollision(false);
            }
        }
    }
}
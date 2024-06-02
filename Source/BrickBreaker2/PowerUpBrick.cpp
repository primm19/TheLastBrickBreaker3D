// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpBrick.h"
#include "EnemyBrick.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "BrickBreakerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerPaddle.h"
#include "Ball.h"


APowerUpBrick::APowerUpBrick()
{
    // Set default values
    BoxCollider->SetBoxExtent(FVector(52.255959, 133.268622, 55.466779));
    BrickMesh->SetWorldScale3D(FVector(1.f, 2.625, 1.0675));

}

void APowerUpBrick::BeginPlay()
{
    Super::BeginPlay();
    
    BrickStrength = 5;

    // Get a reference to the game mode
    GameMode = Cast<ABrickBreakerGameMode>(GetWorld()->GetAuthGameMode());
}

void APowerUpBrick::InitializePowerUp(AEnemyBrick* InOriginalBrick)
{
    OriginalBrick = InOriginalBrick;
    GetWorldTimerManager().SetTimer(DespawnTimer, this, &APowerUpBrick::Despawn, 30.f);
}

void APowerUpBrick::Despawn()
{
    if (OriginalBrick)
    {
        OriginalBrick->SetActorHiddenInGame(false); // Show the original brick again
        OriginalBrick->SetActorEnableCollision(true);
    }
    GetWorld()->GetTimerManager().SetTimer(GameMode->SpawnTimer, GameMode, &ABrickBreakerGameMode::ReplaceWithPowerUpBrick, 15.f);
    Destroy();
}

void APowerUpBrick::HandlePowerUpHit()
{
    // Apply the power-up effect here
    GetWorld()->GetTimerManager().ClearTimer(DespawnTimer);

    GameMode->NumOfBricks--;
    GameMode->LevelChanger();

    RandomFunctionInt = FMath::RandRange(1, 2);

    switch (RandomFunctionInt)
    {
        case 1:
            CallElongatePaddle();
            break;

        case 2:
            CallMultiBall();
            break;
        
        // case 3:
        //     CallSpeed();
        //     break;

        default:
            break;
    }

    if (OriginalBrick)
    {
        OriginalBrick->Destroy(); // Prevent the original brick from respawning
    }

    if (GameMode->NumOfBricks > 0)
    {
        GetWorld()->GetTimerManager().SetTimer(GameMode->SpawnTimer, GameMode, &ABrickBreakerGameMode::ReplaceWithPowerUpBrick, 15.f);
    }

    Destroy(); // Destroy the power-up brick
}

void APowerUpBrick::CallElongatePaddle()
{
    PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetActorOfClass(this, APlayerPaddle::StaticClass()));
    PlayerPaddle->Elongate();
}

void APowerUpBrick::CallMultiBall()
{
    Ball = Cast<ABall>(UGameplayStatics::GetActorOfClass(this, ABall::StaticClass()));
    FVector BallCurrentLocation = Ball->GetActorLocation();
    FRotator BallCurrentRotation = Ball->GetActorRotation();

    GetWorld()->SpawnActor<ABall>(BallClass, BallCurrentLocation, BallCurrentRotation);
    GetWorld()->SpawnActor<ABall>(BallClass, BallCurrentLocation, BallCurrentRotation);
}

// void APowerUpBrick::CallSpeed()
// {
//     TArray<AActor*> FoundBalls;
//     UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall::StaticClass(), FoundBalls);

//     for (AActor* BallIn : FoundBalls)
//     {
//         Ball = Cast<ABall>(BallIn);
//         if (Ball)
//         {
//             Ball->BallSpeed*=8.f;
//             GetWorldTimerManager().SetTimer(NormalizeBallSpeedTimer, this, &APowerUpBrick::NormalizeBallSpeed, 10.f);
//         }
//     }
// }

// void APowerUpBrick::NormalizeBallSpeed()
// {
//     TArray<AActor*> FoundBalls;
//     UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall::StaticClass(), FoundBalls);

//     for (AActor* BallIn : FoundBalls)
//     {
//         Ball = Cast<ABall>(BallIn);
//         if (Ball)
//         {
//             Ball->BallSpeed/=8.f;
//         }
//     }
// }
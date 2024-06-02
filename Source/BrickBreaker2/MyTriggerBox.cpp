// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTriggerBox.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ball.h"
#include "BrickBreakerGameMode.h"
#include "BossBall.h"

AMyTriggerBox::AMyTriggerBox()
{
    
}

void AMyTriggerBox::BeginPlay()
{
    Super::BeginPlay();

    OnActorBeginOverlap.AddDynamic(this, &AMyTriggerBox::Overlapped);
}

void AMyTriggerBox::Overlapped(class AActor* overlappedActor, class AActor* otherActor)
{
    BossBall = Cast<ABossBall>(otherActor);
    if (otherActor && otherActor != this && otherActor != BossBall)
    {
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall::StaticClass(), FoundBalls);
        ball = Cast<ABall>(otherActor);

        int32 numberofActiveBalls = FoundBalls.Num();

        if (numberofActiveBalls == 0)
        {
            BrickBreakerGameMode = Cast<ABrickBreakerGameMode>(UGameplayStatics::GetGameMode(this));
            int32 GetCurrentLevelIndex = BrickBreakerGameMode->CurrentLevelIndex;

            if (BrickBreakerGameMode)
            {
                UGameplayStatics::OpenLevel(GetWorld(), FName(BrickBreakerGameMode->levels[GetCurrentLevelIndex]));
            }
        } else {
            numberofActiveBalls--;
            if (ball)
            {
                ball->Destroy();
            }
        }
    }

    if (BossBall)
    {
        BossBall->Destroy();
    }
}
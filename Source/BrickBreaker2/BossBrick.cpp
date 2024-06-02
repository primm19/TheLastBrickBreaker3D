// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBrick.h"
#include "Components/BoxComponent.h"
#include "BossBall.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
ABossBrick::ABossBrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BossBoxCollider = CreateDefaultSubobject<UBoxComponent>(FName("Boss Box Collider"));
	RootComponent = BossBoxCollider;

	BossMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Boss Mesh"));
	BossMesh->SetupAttachment(BossBoxCollider);

	BallSpawnPoint = CreateDefaultSubobject<USceneComponent>(FName("Ball Spawn Point"));
	BallSpawnPoint->SetupAttachment(BossMesh);
}

// Called when the game starts or when spawned
void ABossBrick::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(BossBallSpawnTimer, this, &ABossBrick::SpawnBossBalls, 3.f, true);
}

// Called every frame
void ABossBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossBrick::SpawnBossBalls()
{
	// Check if the boss is still alive
	if (IsBossAlive())
	{
		// Spawn 3 balls in a row
		for (int i = 0; i < 3; i++)
		{
			FVector Location = BallSpawnPoint->GetComponentLocation();
			FRotator Rotation = BallSpawnPoint->GetComponentRotation();

			// Add a random offset to the rotation
			Rotation.Yaw += FMath::RandRange(-45, 45);

			GetWorld()->SpawnActor<ABossBall>(BossBallClass, Location, Rotation);
		}
	}
	else
	{
		// If the boss is dead, clear the timer
		GetWorldTimerManager().ClearTimer(BossBallSpawnTimer);
	}
}

bool ABossBrick::IsBossAlive()
{
	// Implement this function to check if the boss is still alive
	// For example, you could check if the boss's health is greater than 0
	return true;
}
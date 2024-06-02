// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossBrick.generated.h"

UCLASS()
class BRICKBREAKER2_API ABossBrick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossBrick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* BossMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnBossBalls();

private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BossBoxCollider;

	UPROPERTY(EditAnywhere)
	class USceneComponent* BallSpawnPoint;

	FTimerHandle BossBallSpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ABossBall> BossBallClass;

	bool IsBossAlive();

};

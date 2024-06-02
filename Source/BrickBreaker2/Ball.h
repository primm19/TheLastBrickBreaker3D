// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

UCLASS()
class BRICKBREAKER2_API ABall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereCollider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BallMesh;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	FVector ClampVectorSize(const FVector& InputVector, float Min, float Max);

	class ABrickBreakerGameMode* BrickBreakerGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
	class ABossBrick* Boss;

	class APlayerPaddle* PlayerPaddle;

	UPROPERTY(EditAnywhere)
	float Damage = 5.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float BallSpeed = -400000.f;
	float MaxSpeed = 4000.f;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/SphereComponent.h"
#include "PlayerPaddle.h"
#include "Math/Vector.h"
#include <cmath>
#include "EnemyBrick.h"
#include "BrickBreakerGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/TriggerBox.h"
#include "PowerUpBrick.h"
#include "BossBrick.h"
#include "GameFramework/DamageType.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(FName("Sphere Collider"));
	RootComponent = SphereCollider;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Ball Mesh"));
	BallMesh->SetupAttachment(SphereCollider);
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->AddImpulse(FVector(0.f, 0.f, BallSpeed));
	
	SphereCollider->OnComponentHit.AddDynamic(this, &ABall::OnHit);

	BrickBreakerGameMode = Cast<ABrickBreakerGameMode>(UGameplayStatics::GetGameMode(this));
	PlayerPaddle = Cast<APlayerPaddle>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ABall::ClampVectorSize(const FVector& InputVector, float Min, float Max)
{
	float VectorSize = InputVector.Size();
    if (VectorSize < Min)
    {
        return InputVector.GetSafeNormal() * Min;
    }
    else if (VectorSize > Max)
    {
        return InputVector.GetSafeNormal() * Max;
    }
    return InputVector;
}

void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (SphereCollider)
	{
		FVector CurrentLinearVel = SphereCollider->GetPhysicsLinearVelocity();
		BallSpeed = 1500.f;
		BallSpeed+=5;
		FVector CurrentLinearVelClamped = ClampVectorSize(CurrentLinearVel, BallSpeed, MaxSpeed);
		SphereCollider->SetPhysicsLinearVelocity(CurrentLinearVelClamped);

		FVector ImpactPointVector = Hit.ImpactPoint;
		float ImpactPointY = ImpactPointVector.Y;

		AEnemyBrick* Brick = Cast<AEnemyBrick>(OtherActor);

		if (Brick)
		{
			Brick->BrickStrength--;
			Brick->UpdateBrickColor();

			if (Brick->BrickStrength == 0)
			{
				BrickBreakerGameMode->NumOfBricks--;
				BrickBreakerGameMode->LevelChanger();
			}
		}

		APowerUpBrick* PowerUpBrick = Cast<APowerUpBrick>(OtherActor);

		if (PowerUpBrick)
		{
			PowerUpBrick->HandlePowerUpHit();
		}

		// PlayerPaddle = Cast<APlayerPaddle>(OtherActor);

		// if (PlayerPaddle)
		// {
		// 	FVector PaddleLocation = OtherActor->GetActorLocation();
		// 	float PaddleCenterY = PaddleLocation.Y;
		
		// 	if (ImpactPointY < PaddleCenterY)
		// 	{
		// 		BallSpeed = 1500.f;
		// 		BallSpeed += 5;
		// 		float CurrentLinearVelY = CurrentLinearVelClamped.Y;
		// 		float CurrentLinearVelZ = CurrentLinearVelClamped.Z;

		// 		CurrentLinearVelY = FMath::Clamp(CurrentLinearVelY, BallSpeed, MaxSpeed);
		// 		CurrentLinearVelZ = FMath::Clamp(CurrentLinearVelZ, BallSpeed, MaxSpeed);
		// 		CurrentLinearVel = FVector(CurrentLinearVel.X, -CurrentLinearVelY, CurrentLinearVelZ);

		// 		SphereCollider->SetPhysicsLinearVelocity(CurrentLinearVel);

		// 	} else if (ImpactPointY == PaddleCenterY || FMath::Abs(ImpactPointVector.Y - PaddleLocation.Y) <= 50.f) {
		// 		SphereCollider->SetPhysicsLinearVelocity(CurrentLinearVel);
		// 	} else {
		// 		BallSpeed = 1500.f;
		// 		BallSpeed += 5;
		// 		float CurrentLinearVelY = CurrentLinearVelClamped.Y;
		// 		float CurrentLinearVelZ = CurrentLinearVelClamped.Z;

		// 		CurrentLinearVelY = FMath::Clamp(CurrentLinearVelY, BallSpeed, MaxSpeed);
		// 		CurrentLinearVelZ = FMath::Clamp(CurrentLinearVelZ, BallSpeed, MaxSpeed);
		// 		CurrentLinearVel = FVector(CurrentLinearVel.X, CurrentLinearVelY, CurrentLinearVelZ);

		// 		SphereCollider->SetPhysicsLinearVelocity(CurrentLinearVel);
		// 	}
		// }
		
		Boss = Cast<ABossBrick>(OtherActor);
		UClass* DamageTypeClass = UDamageType::StaticClass();
		if (Boss && OtherActor != this)
		{
			TArray<AActor*> FoundPaddles;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerPaddle::StaticClass(), FoundPaddles);
			for (AActor* Paddle : FoundPaddles)
			{
				PlayerPaddle = Cast<APlayerPaddle>(Paddle);

				if (PlayerPaddle)
				{
					UGameplayStatics::ApplyDamage(Boss, Damage, PlayerPaddle->GetController(), this, DamageTypeClass);
				}
			}
		}
	}
}

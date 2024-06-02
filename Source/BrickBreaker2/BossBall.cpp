// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBall.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PlayerPaddle.h"
#include "Kismet/GameplayStatics.h"
#include "BossBrick.h"

// Sets default values
ABossBall::ABossBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Ball Mesh"));
	RootComponent = BallMesh;

	Mover = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Ball Movement Component"));
	Mover->InitialSpeed = 1500.f;
	Mover->MaxSpeed = 1500.f;
}

// Called when the game starts or when spawned
void ABossBall::BeginPlay()
{
	Super::BeginPlay();
	
	BallMesh->OnComponentHit.AddDynamic(this, &ABossBall::OnHit);
}

// Called every frame
void ABossBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ABossBall::ClampVectorSize(const FVector& InputVector, float Min, float Max)
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

void ABossBall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (BallMesh)
	{
		FVector CurrentLinearVelocity = BallMesh->GetPhysicsLinearVelocity();
		float BallSpeed = 1500.f;
		BallSpeed +=5;
		FVector ClampedVelocity = ClampVectorSize(CurrentLinearVelocity, BallSpeed, BallSpeed);
		BallMesh->SetPhysicsLinearVelocity(ClampedVelocity);

		APlayerPaddle* PlayerPaddle = Cast<APlayerPaddle>(OtherActor);
		UClass* DamageTypeClass = UDamageType::StaticClass();
		if (PlayerPaddle)
		{
			AController* BallInstigator = nullptr;
			UGameplayStatics::ApplyDamage(PlayerPaddle, Damage, BallInstigator, this, DamageTypeClass);
			
		}
	}
}
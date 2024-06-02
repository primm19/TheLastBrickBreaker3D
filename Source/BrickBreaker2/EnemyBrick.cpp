// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBrick.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AEnemyBrick::AEnemyBrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(FName("Box Collider"));
	RootComponent = BoxCollider;

	BrickMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Brick Mesh"));
	BrickMesh->SetupAttachment(BoxCollider);

	MaterialRed = CreateDefaultSubobject<UMaterialInterface>("MaterialRed");
	MaterialPurple = CreateDefaultSubobject<UMaterialInterface>("MaterialPurple");
	MaterialGreen = CreateDefaultSubobject<UMaterialInterface>("MaterialGreen");
	MaterialYellow = CreateDefaultSubobject<UMaterialInterface>("MaterialYellow");
	MaterialBlack = CreateDefaultSubobject<UMaterialInterface>("MaterialBlack");

	OrbitSpeed = 350.f;
	OrbitRadius = 2.5;
	
}

void AEnemyBrick::HandleDestruction()
{
	//TODO: Spawn Particle Emitter

	Destroy();
}

// Called when the game starts or when spawned
void AEnemyBrick::BeginPlay()
{
	Super::BeginPlay();
	
	UpdateBrickColor();

}

// Called every frame
void AEnemyBrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If the brick should move, update its position
    if (bShouldMove && OrbitCenter)
    {
		Orbit();
    }
}

void AEnemyBrick::UpdateBrickColor()
{
	switch (BrickStrength)
	{
	case 0:
		HandleDestruction();
		break;
	
	case 1:
		BrickMesh->SetMaterial(0, MaterialRed);
		break;

	case 2:
		BrickMesh->SetMaterial(0, MaterialPurple);
		break;

	case 3:
		BrickMesh->SetMaterial(0, MaterialGreen);
		break;

	case 4:
		BrickMesh->SetMaterial(0, MaterialYellow);
		break;

	case 5:
		BrickMesh->SetMaterial(0, MaterialBlack);
		break;
	
	default:
		break;
	}
}

void AEnemyBrick::Orbit()
{
	// Calculate the new angle based on the current time and the orbit speed
    float NewAngle = GetWorld()->GetTimeSeconds() * OrbitSpeed + AngleOffset;

    // Calculate the new X and Y positions based on the angle and the orbit radius
    float NewY = OrbitCenter->GetActorLocation().Y + FMath::Cos(NewAngle) * OrbitRadius;
    float NewZ = OrbitCenter->GetActorLocation().Z + FMath::Sin(NewAngle) * OrbitRadius;

    // Update the actor's location
    SetActorRelativeLocation(FVector(GetActorLocation().X, NewY, NewZ));
}
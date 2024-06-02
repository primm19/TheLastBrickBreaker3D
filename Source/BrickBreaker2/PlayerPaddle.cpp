// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPaddle.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
APlayerPaddle::APlayerPaddle()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(FName("Capsule Collider"));
	RootComponent = CapsuleCollider;

	CapsuleMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Capsule Mesh"));
	CapsuleMesh->SetupAttachment(CapsuleCollider);
}

// Called when the game starts or when spawned
void APlayerPaddle::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());
	
	if (PlayerControllerRef)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerRef->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

// Called every frame
void APlayerPaddle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPaddle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerPaddle::Move);
    }
}

void APlayerPaddle::Move(const FInputActionValue& Value)
{
    const float CurrentValue = Value.Get<float>();

    if (CurrentValue)
    {
        FVector CurrentLocation = GetActorLocation();

        float MovementSpeed = 1500.f;
        float MovementDistance = CurrentValue * MovementSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);

        FVector NewLocation = CurrentLocation + FVector(0.f, MovementDistance, 0.f);

        float MinY = -1800.f;
        float MaxY = 1800.f;

        NewLocation.Y = FMath::Clamp(NewLocation.Y, MinY, MaxY);

        SetActorLocation(NewLocation);
    }
}

void APlayerPaddle::Elongate()
{
	FVector PaddleScale = GetActorScale3D();
    PaddleScale.Z*=1.5;
    SetActorScale3D(FVector(PaddleScale.X, PaddleScale.Y, PaddleScale.Z));
	GetWorldTimerManager().SetTimer(ShrinkTimer, this, &APlayerPaddle::Shrink, 9.f);
}

void APlayerPaddle::Shrink()
{
	SetActorScale3D(FVector(1.f, 1.f, 1.f));
}
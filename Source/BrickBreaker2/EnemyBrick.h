// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBrick.generated.h"

UCLASS()
class BRICKBREAKER2_API AEnemyBrick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBrick();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BrickMesh;

	void HandleDestruction();

	void UpdateBrickColor();

	UPROPERTY(EditAnywhere)
	int32 BrickStrength;

	UPROPERTY(EditAnywhere, Category="Materials")
	UMaterialInterface* MaterialBlack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    bool bShouldMove;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float OrbitRadius;

    // Add UPROPERTY for the orbit speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float OrbitSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float AngleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	AEnemyBrick* OrbitCenter;

	void Orbit();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Materials")
	UMaterialInterface* MaterialRed;

	UPROPERTY(EditAnywhere, Category="Materials")
	UMaterialInterface* MaterialPurple;

	UPROPERTY(EditAnywhere, Category="Materials")
	UMaterialInterface* MaterialGreen;

	UPROPERTY(EditAnywhere, Category="Materials")
	UMaterialInterface* MaterialYellow;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

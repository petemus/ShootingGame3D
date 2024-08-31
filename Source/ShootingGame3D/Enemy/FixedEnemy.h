// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "FixedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API AFixedEnemy : public AEnemy
{
	GENERATED_BODY()
	
public:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	virtual void Move(float DeltaTime) override;


public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> FirePoses;
};

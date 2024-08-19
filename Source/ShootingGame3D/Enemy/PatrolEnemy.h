// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "PatrolEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API APatrolEnemy : public AEnemy
{
	GENERATED_BODY()
public:
	APatrolEnemy();

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	virtual void Move(float DeltaTime) override;
};

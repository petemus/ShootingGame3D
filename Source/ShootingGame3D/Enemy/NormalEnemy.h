// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "NormalEnemy.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API ANormalEnemy : public AEnemy
{
	GENERATED_BODY()
public:
	ANormalEnemy();

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	virtual void Attack_Implementation() override;

	virtual void Move(float DeltaTime) override;
};

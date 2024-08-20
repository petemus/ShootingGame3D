// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBullet.h"
#include "DirectEnemyBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API ADirectEnemyBullet : public AEnemyBullet
{
	GENERATED_BODY()
	
public:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Move(float Delta) override;


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"

#include "BossController.generated.h"

UCLASS()
class SHOOTINGGAME3D_API ABossController : public AEnemyController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

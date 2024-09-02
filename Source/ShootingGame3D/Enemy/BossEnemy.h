// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "BossEnemy.generated.h"

class ADirectEnemyBullet;

UCLASS()
class SHOOTINGGAME3D_API ABossEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Attack1();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> FirePoses;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ADirectEnemyBullet> Bullet;

public:
	FTimerHandle Attack1Timer;


public:
	int32 Attack1Inx = 0;
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBullet.h"
#include "InDirectEnemyBullet.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API AInDirectEnemyBullet : public AEnemyBullet
{
	GENERATED_BODY()
public:

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(BlueprintReadWrite)
	FVector TargetPosition;

	UPROPERTY(BlueprintReadWrite)
	FVector StartPosition;

	UPROPERTY(BlueprintReadWrite)
	float TotalTime;

	UPROPERTY(BlueprintReadWrite)
	float StartTime;

	UPROPERTY(BlueprintReadWrite)
	FVector HighPosition;

public:
	virtual void Move(float Delta) override;

	UFUNCTION(BlueprintCallable)
	void InitialVar(FVector Target, FVector High, float Time);

};

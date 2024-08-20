// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShootingGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API UShootingGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UShootingGameInstance();

private:
	/*  */
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 Gold;

public:
	UPROPERTY(BlueprintReadWrite)
	int32 Health;

	UPROPERTY(BlueprintReadWrite)
	int32 Attack;

	UPROPERTY(BlueprintReadWrite)
	float AttackDur;

	UPROPERTY(BlueprintReadWrite)
	float MoveSpeed;

public:
	/* Gold Used Func */
	UFUNCTION(BlueprintCallable)
	void AddGold(int32 Amount);


	UFUNCTION(BlueprintCallable)
	bool SpendGold(int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void IncreaseHealth();
	
	UFUNCTION(BlueprintCallable)
	void IncreaseAttack();	
	
	UFUNCTION(BlueprintCallable)
	void IncreaseAttackDur();

	UFUNCTION(BlueprintCallable)
	void IncreaseSpeed();
public:
	/* Get Set */
	UFUNCTION(BlueprintCallable)
	int32 GetGold() { return Gold; }

	UFUNCTION(BlueprintCallable)
	void SetGold(int32 Amount) { Gold = Amount; }
	
};

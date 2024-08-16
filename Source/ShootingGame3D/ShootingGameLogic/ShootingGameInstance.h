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
	/* Gold Used Func */
	UFUNCTION(BlueprintCallable)
	void AddGold(int32 Amount);


	UFUNCTION(BlueprintCallable)
	bool SpendGold(int32 Amount);

public:
	/* Get Set */
	UFUNCTION(BlueprintCallable)
	int32 GetGold() { return Gold; }

	UFUNCTION(BlueprintCallable)
	void SetGold(int32 Amount) { Gold = Amount; }
	
};

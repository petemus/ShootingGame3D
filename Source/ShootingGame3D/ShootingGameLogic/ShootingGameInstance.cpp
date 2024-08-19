// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameInstance.h"

UShootingGameInstance::UShootingGameInstance()
{
}

void UShootingGameInstance::AddGold(int32 Amount)
{
	Gold += Amount;
}

bool UShootingGameInstance::SpendGold(int32 Amount)
{
	if (Gold >= Amount)
	{
		Gold -= Amount;
		return true;
	}
	return false;
}

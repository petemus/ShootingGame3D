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

void UShootingGameInstance::IncreaseHealth()
{
	Health += 5;
	if (Health >= 25)
	{
		Health = 25;
	}
}

void UShootingGameInstance::IncreaseAttack()
{
	Attack += 1;
	if (Attack >= 5)
	{
		Attack = 5;
	}
}

void UShootingGameInstance::IncreaseAttackDur()
{
	AttackDur -= -0.2f;

	if (AttackDur <= -0.8f)
	{
		AttackDur = -0.8f;
	}
}

void UShootingGameInstance::IncreaseSpeed()
{
	MoveSpeed += 40.f;

	if (MoveSpeed >= 200.f)
	{
		MoveSpeed = 200.f;
	}
}

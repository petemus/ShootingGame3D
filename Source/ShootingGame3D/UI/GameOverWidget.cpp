// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverWidget.h"
#include "ShootingGameModeBase.h"

void UGameOverWidget::SetStartMode(AShootingGameModeBase* mode)
{
	if (mode != nullptr)
	{
		GameMode = mode;
	}
}

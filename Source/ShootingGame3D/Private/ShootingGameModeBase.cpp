// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "ShootingGame3D/UI/GameOverWidget.h"


void AShootingGameModeBase::BeginPlay()
{
	if (GameOverWidget != nullptr)
	{
		GameOverUI = CreateWidget<UGameOverWidget>(GetWorld(), GameOverWidget);

		if (GameOverUI != nullptr)
		{
			GameOverUI->SetStartMode(this);
		}
	}
}

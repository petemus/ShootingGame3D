// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "ShootingGame3D/UI/GameOverWidget.h"

#include "ShootingGame3D/UI/PlayerHUD.h"


void AShootingGameModeBase::BeginPlay()
{
	if (GameOverWidget != nullptr)
	{
		GameOverUI = CreateWidget<UGameOverWidget>(GetWorld(), GameOverWidget);

		if (GameOverUI != nullptr)
		{
			GameOverUI->SetStartMode(this);
			GameOverUI->AddToViewport();
			GameOverUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UPlayerHUD>(GetWorld(), HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
			HUDWidget->InitWidget();
		}
	}
}

void AShootingGameModeBase::MiniMapSet(int32 RoomIdx, uint8 OpenFlag)
{
	if (HUDWidget)
	{
		HUDWidget->SetCurrentRoom(RoomIdx, OpenFlag);
		//HUDWidget->SetHp(5);
	}
}

void AShootingGameModeBase::HpSet(int32 CurrentHp)
{
	if (HUDWidget)
	{
		HUDWidget->SetHp(CurrentHp);
	}
}

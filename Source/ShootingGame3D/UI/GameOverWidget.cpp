// Fill out your copyright notice in the Description page of Project Settings.

#include "GameOverWidget.h"
#include "ShootingGame3D/Public/ShootingGameModeBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	if (BTN_MainMenu)
	{
		BTN_MainMenu->OnClicked.AddDynamic(this, &UGameOverWidget::BackToStartMenu);
	}
}

void UGameOverWidget::BackToStartMenu()
{
	AShootingGameModeBase* GM = Cast<AShootingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->BackToStartLevel();
	}
}

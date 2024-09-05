// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClearWidget.h"

#include "ShootingGame3D/Public/ShootingGameModeBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameClearWidget::NativeConstruct()
{
	if (BTN_BackToMain)
	{
		BTN_BackToMain->OnClicked.AddDynamic(this, &UGameClearWidget::BackToStartLevel);
	}
}

void UGameClearWidget::BackToStartLevel()
{
	AShootingGameModeBase* GM = Cast<AShootingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->BackToStartLevel();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BossUI.h"

#include "Components/ProgressBar.h"

void UBossUI::NativeConstruct()
{
	Super::NativeConstruct();
	BossHealthBar->SetPercent(1.f);
}

void UBossUI::UpdateHealth(float percent)
{
	BossHealthBar->SetPercent(percent);
}

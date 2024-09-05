// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "ShootingGame3D/ShootingGameLogic/StartGameMode.h"
#include "ShopWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void UStartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	StartButton->OnClicked.AddDynamic(this, &UStartWidget::Start);
}

void UStartWidget::Start()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
}


void UStartWidget::SetStartMode(AStartGameMode* mode)
{
	if (mode != nullptr)
	{
		StartMode = mode;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "Components/Button.h"
#include "ShootingGame3D/ShootingGameLogic/StartGameMode.h"
#include "StartWidget.h"
#include "Engine/GameInstance.h"
#include "../ShootingGameLogic/ShootingGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	QuitButton->OnClicked.AddDynamic(this, &UShopWidget::Quit);
}



void UShopWidget::Quit()
{
	// TODO
	if (StartMode == nullptr)
		return;
	if (StartMode->StartUI == nullptr)
		return;
	StartMode->StartUI->AddToViewport();

	RemoveFromParent();
}

void UShopWidget::SetStartMode(AStartGameMode* mode)
{
	if (mode != nullptr)
	{
		StartMode = mode;
	}
}

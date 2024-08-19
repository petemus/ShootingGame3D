// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameMode.h"
#include "Blueprint/UserWidget.h"
#include "ShootingGame3D/UI/StartWidget.h"
#include "ShootingGame3D/UI/ShopWidget.h"
#include "GameFramework/PlayerController.h"

void AStartGameMode::BeginPlay()
{
	if (StartWidget != nullptr)
	{
		StartUI = CreateWidget<UStartWidget>(GetWorld(), StartWidget);

		if (StartUI != nullptr)
		{
			StartUI->SetStartMode(this);
			StartUI->AddToViewport();
		}
	}
	if (ShopWidget != nullptr)
	{
		ShopUI = CreateWidget<UShopWidget>(GetWorld(), ShopWidget);

		if (ShopUI != nullptr)
		{
			ShopUI->SetStartMode(this);
		}
	}
	ShowMouse();
}

void AStartGameMode::ShowMouse()
{
	APlayerController* PController = GetWorld()->GetFirstPlayerController();
	if (PController)
	{
		PController->bShowMouseCursor = true;
		PController->bEnableClickEvents = true;
	}
}
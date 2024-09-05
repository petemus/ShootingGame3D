// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameModeBase.h"
#include "ShootingGame3D/Gimmick/DungeonGeneratorComponent.h"
#include "ShootingGame3D/UI/GameOverWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingGame3D/UI/PlayerHUD.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "ShootingGame3D/UI/GameClearWidget.h"

AShootingGameModeBase::AShootingGameModeBase()
{
	DungeonGenerator = CreateDefaultSubobject<UDungeonGeneratorComponent>(TEXT("DungeonGenerator"));
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
}

void AShootingGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (GameOverWidget != nullptr)
	{
		GameOverUI = CreateWidget<UGameOverWidget>(GetWorld(), GameOverWidget);

		if (GameOverUI != nullptr)
		{
			GameOverUI->AddToViewport();
			GameOverUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (GameClearWidget != nullptr)
	{
		GameClearUI = CreateWidget<UGameClearWidget>(GetWorld(), GameClearWidget);

		if (GameClearUI != nullptr)
		{
			GameClearUI->AddToViewport();
			GameClearUI->SetVisibility(ESlateVisibility::Hidden);
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

	if (DungeonSound)
	{
		AudioComponent->SetSound(DungeonSound);
		AudioComponent->Play();
	}
}

void AShootingGameModeBase::MiniMapSet(int32 RoomIdx, uint8 OpenFlag)
{
	if (HUDWidget)
	{
		HUDWidget->SetCurrentRoom(RoomIdx, OpenFlag);
	}
}

void AShootingGameModeBase::HpSet(int32 CurrentHp)
{
	if (HUDWidget)
	{
		HUDWidget->SetHp(CurrentHp);
	}
}

void AShootingGameModeBase::GameOver()
{
	if (GameOverUI)
	{
		GameOverUI->SetVisibility(ESlateVisibility::Visible);
		
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			FInputModeUIOnly InputMode;
			PC->bShowMouseCursor = true;
			PC->SetInputMode(InputMode);
		}
	}
}

void AShootingGameModeBase::GameClaer()
{
	if (GameClearUI)
	{
		GameClearUI->SetVisibility(ESlateVisibility::Visible);

		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			FInputModeUIOnly InputMode;
			PC->bShowMouseCursor = true;
			PC->SetInputMode(InputMode);
		}
	}

	AudioComponent->Stop();
}

void AShootingGameModeBase::BackToStartLevel()
{
	// Start Level Open
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ShootingStartLevel"));
	AudioComponent->Stop();
}

void AShootingGameModeBase::PlayBossSound()
{
	if (BossSound)
	{
		AudioComponent->Stop();
		AudioComponent->SetSound(BossSound);
		AudioComponent->Play();
	}
}
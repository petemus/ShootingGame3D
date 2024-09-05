// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRoom.h"
#include "ShootingGame3D/Public/ShootingGameModeBase.h"
#include "Kismet/GameplayStatics.h"

ABossRoom::ABossRoom()
{
}

void ABossRoom::Tick(float DeltaTime)
{
}

void ABossRoom::EndRoom()
{
	AShootingGameModeBase* GM = Cast<AShootingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->GameClaer();
	}
}

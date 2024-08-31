// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootingGameModeBase.generated.h"

class UGameOverWidget;

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API AShootingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf< UGameOverWidget> GameOverWidget;
	
	UGameOverWidget* GameOverUI;
};

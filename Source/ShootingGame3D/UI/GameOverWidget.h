// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class AShootingGameModeBase;

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	AShootingGameModeBase* GameMode;

	/* Get Set */
	AShootingGameModeBase* GetStartMode() { return GameMode; }

	void SetStartMode(AShootingGameModeBase* mode);
};

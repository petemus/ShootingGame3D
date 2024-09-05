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
	AShootingGameModeBase();
	virtual void BeginPlay() override;
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UGameOverWidget> GameOverWidget;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class UGameClearWidget> GameClearWidget;
	
	UGameOverWidget* GameOverUI;
	class UGameClearWidget* GameClearUI;

public:
	void MiniMapSet(int32 RoomIdx, uint8 OpenFlag);
	void HpSet(int32 CurrentHp);
	void GameOver();
	void GameClaer();
	void BackToStartLevel();
	void PlayBossSound();

private:
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr<class UAudioComponent> AudioComponent;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<class UDungeonGeneratorComponent> DungeonGenerator;

private:
	UPROPERTY(EditAnywhere, Category = "Widget")
	TSubclassOf<class UPlayerHUD> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UPlayerHUD> HUDWidget;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<class USoundBase> DungeonSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<class USoundBase> BossSound;
};

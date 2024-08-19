// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StartGameMode.generated.h"

class UStartWidget;
class UShopWidget;

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ShowMouse();

public:
	UPROPERTY(BlueprintReadWrite , EditAnywhere)
	TSubclassOf< UStartWidget> StartWidget;

	UPROPERTY(BlueprintReadWrite , EditAnywhere)
	TSubclassOf< UShopWidget> ShopWidget;

	UStartWidget* StartUI;
	UShopWidget* ShopUI;



};

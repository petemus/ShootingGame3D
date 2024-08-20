// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

class UButton;
class AStartGameMode;

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

protected:

public:
	/* Button */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* AttackStatButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* MoveStatButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* AttackDurationButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* QuitButton;

public:
	/* Cache Obj*/
	AStartGameMode* StartMode;

public:
	/* Play Button Func */
	UFUNCTION()
	void AttackIncrease();

	UFUNCTION()
	void MoveIncrease();

	UFUNCTION()
	void AttackDurationDecrease();

	UFUNCTION()
	void Quit();

	/* Get Set */
	AStartGameMode* GetStartMode() { return StartMode; }

	void SetStartMode(AStartGameMode* mode);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartWidget.generated.h"

class UButton;
class AStartGameMode;


/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API UStartWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;


public:
	/* Button */
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ShopButton;

	/* Cache */
	UPROPERTY()
	AStartGameMode* StartMode;

public:
	/* Play Button Func */
	UFUNCTION()
	void Start();


	UFUNCTION()
	void OpenShop();

	/* Get Set */
	AStartGameMode* GetStartMode() { return StartMode; }

	void SetStartMode(AStartGameMode* mode);

};

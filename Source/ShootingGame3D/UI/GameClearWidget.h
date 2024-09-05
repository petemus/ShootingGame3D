// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameClearWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API UGameClearWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void BackToStartLevel();

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BTN_BackToMain;
};

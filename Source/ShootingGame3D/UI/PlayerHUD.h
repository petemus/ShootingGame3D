// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitWidget();
	void SetCurrentRoom(int32 InIdx, uint8 OpenFlag);
	void SetHp(int32 CurrentHp);

private:
	UPROPERTY(VisibleAnywhere, Category = "Minimap")
	TArray<TObjectPtr<class UImage>> Roads;

	UPROPERTY(VisibleAnywhere, Category = "Minimap")
	TArray<TObjectPtr<class UImage>> Hides;

	UPROPERTY(VisibleAnywhere, Category = "Minimap")
	TArray<TObjectPtr<class UImage>> Hps;

private:
	UPROPERTY(EditAnywhere, Category = "Texture")
	TMap<uint8, TObjectPtr<class UTexture2D>> RoadTexture;

	UPROPERTY(EditAnywhere, Category = "Texture")
	TMap<FName, TObjectPtr<class UTexture2D>> HideTexture;

	UPROPERTY(EditAnywhere, Category = "Texture")
	TMap<int32, TObjectPtr<class UTexture2D>> HpTexture;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentRoomIdx = 0;
	
	bool bIsStart = true;
};

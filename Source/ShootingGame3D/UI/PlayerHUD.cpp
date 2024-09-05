// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"

void UPlayerHUD::InitWidget()
{
	TArray<UWidget*> TempWidgets;
	WidgetTree->GetAllWidgets(TempWidgets);

	for (UWidget* Widget : TempWidgets)
	{
		if (Widget->GetName().Contains(TEXT("Road")))
		{
			UImage* RoadImage = Cast<UImage>(Widget);
			if (RoadImage)
			{
				Roads.Push(RoadImage);
			}
		}
		else if (Widget->GetName().Contains(TEXT("Hide")))
		{
			UImage* HideImage = Cast<UImage>(Widget);
			if (HideImage)
			{
				Hides.Push(HideImage);
			}
		}
		else if (Widget->GetName().Contains(TEXT("Hp")))
		{
			UImage* HpImage = Cast<UImage>(Widget);
			if (HpImage)
			{
				Hps.Push(HpImage);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%d, %d, %d"), Roads.Num(), Hides.Num(), Hps.Num());
}

void UPlayerHUD::SetCurrentRoom(int32 InIdx, uint8 OpenFlag)
{
	if (bIsStart && InIdx == 0)
	{
		// 현재 방 설정
		CurrentRoomIdx = InIdx;
		// 현재 방 밝은 화면으로 변환
		Hides[CurrentRoomIdx]->SetBrushFromTexture(HideTexture[TEXT("Visible")]);
		bIsStart = false;
	}

	if (CurrentRoomIdx != InIdx)
	{
		// 이전 방 어두운 화면으로 변환
		Hides[CurrentRoomIdx]->SetBrushFromTexture(HideTexture[TEXT("Hide")]);
		// 현재 방 설정
		CurrentRoomIdx = InIdx;
		// 현재 방 밝은 화면으로 변환
		Hides[CurrentRoomIdx]->SetBrushFromTexture(HideTexture[TEXT("Visible")]);
	}

	// 방의 이미지 설정
	Roads[CurrentRoomIdx]->SetBrushFromTexture(RoadTexture[OpenFlag]);
}

void UPlayerHUD::SetHp(int32 CurrentHp)
{
	if (CurrentHp > 6 || CurrentHp < 0) return;
	// 그림 : 1: half, 2: full, 3: empty
	// 하트 인덱스 :      0    1    2
	// 체력         0   1 2  3 4  5 6
	// 인덱스                        
	// 현재 인덱스 구하기
	int32 Idx = (CurrentHp - 1) / 2;
	

	if (CurrentHp == 0)
	{
		for (UImage* HpImg : Hps)
		{
			HpImg->SetBrushFromTexture(HpTexture[3]);
		}
	}
	else
	{
		for (int i = Idx; i < Hps.Num(); i++)
		{
			if (i == Idx)
			{
				Hps[i]->SetBrushFromTexture(HpTexture[((CurrentHp - 1) % 2) + 1]);
			}
			else
			{
				Hps[i]->SetBrushFromTexture(HpTexture[3]);
			}
		}
	}
}
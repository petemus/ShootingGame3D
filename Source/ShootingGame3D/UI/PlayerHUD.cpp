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
	if (CurrentHp > 6) return;

	int32 Idx = (CurrentHp - 1) / 2;
	
	for (int32 i = 0; i < 3; i++)
	{
		if (i + 1< Idx)
		{
			// 하트 가득 채우기
			Hps[Idx]->SetBrushFromTexture(HpTexture[1]);
		}
		else if (i + 1 == Idx)
		{
			// 체력에 따라 채우기 (짝수면 가득, 홀수면 반칸)
			Hps[Idx]->SetBrushFromTexture(HpTexture[CurrentHp % 2] + 1);
		}
		else if (i + 1 > Idx)
		{
			// 모두 비우기
			Hps[Idx]->SetBrushFromTexture(HpTexture[3]);
		}
	}
}
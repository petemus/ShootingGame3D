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
		// ���� �� ����
		CurrentRoomIdx = InIdx;
		// ���� �� ���� ȭ������ ��ȯ
		Hides[CurrentRoomIdx]->SetBrushFromTexture(HideTexture[TEXT("Visible")]);
		bIsStart = false;
	}

	if (CurrentRoomIdx != InIdx)
	{
		// ���� �� ��ο� ȭ������ ��ȯ
		Hides[CurrentRoomIdx]->SetBrushFromTexture(HideTexture[TEXT("Hide")]);
		// ���� �� ����
		CurrentRoomIdx = InIdx;
		// ���� �� ���� ȭ������ ��ȯ
		Hides[CurrentRoomIdx]->SetBrushFromTexture(HideTexture[TEXT("Visible")]);
	}

	// ���� �̹��� ����
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
			// ��Ʈ ���� ä���
			Hps[Idx]->SetBrushFromTexture(HpTexture[1]);
		}
		else if (i + 1 == Idx)
		{
			// ü�¿� ���� ä��� (¦���� ����, Ȧ���� ��ĭ)
			Hps[Idx]->SetBrushFromTexture(HpTexture[CurrentHp % 2] + 1);
		}
		else if (i + 1 > Idx)
		{
			// ��� ����
			Hps[Idx]->SetBrushFromTexture(HpTexture[3]);
		}
	}
}
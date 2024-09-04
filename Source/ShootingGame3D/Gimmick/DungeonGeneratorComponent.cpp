// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGeneratorComponent.h"

#include "RoomBase.h"

UDungeonGeneratorComponent::UDungeonGeneratorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Default Value Set
	Width = 4;
	Height = 3;
	RoomSize = 2800.0f;
}



void UDungeonGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Generate Dungeon Map Info
	GenerateMaze();

	SpawnMaze();
}

void UDungeonGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UDungeonGeneratorComponent::GenerateMaze()
{
	// 1. Height * Width ��ŭ �迭 �ʱ�ȭ
	//    = Room ���� �� ������ ��ġ �ʱ�ȭ
	for (int32 y = 0; y < Height; y++)
	{
		for (int32 x = 0; x < Width; x++)
		{
			FRoomInfo Info;
			Info.RoomNum = (y * Width) + x;
			Info.OpenDir = static_cast<uint8>(EOpenDir::EOD_NONE);
			Info.bIsVisited = false;

			DungeonMaps.Add(Info);
		}
	}

	// �������� üũ ������ �� ���
	TArray<int32> TempRooms;

	// ���� ��ġ(0,0)�� �Ʒ�, ������ üũ�ؼ� Ž�� ������ �� �迭�� �߰��ϱ�
	DungeonMaps[0].bIsVisited = true;
	TempRooms.Add(Width);
	TempRooms.Add(1);

	int32 CheckCnt = 1;

	// 2. ��� ���� üũ�� �� ������ �ݺ��ؼ� �̷� �����ϱ�
	while (CheckCnt != DungeonMaps.Num())
	{
		// ������ �� �̱�
		int32 RandIdx = TempRooms[FMath::RandRange(0, TempRooms.Num() - 1)];

		// �ش� �� �湮ó��
		DungeonMaps[RandIdx].bIsVisited = true;
		// ī��Ʈ ����
		CheckCnt++;
		// �ش� �� �ӽ� �� ��Ͽ��� ����
		TempRooms.Remove(RandIdx);

		// �ش� �����κ��� �����¿� üũ�Ͽ� �������� ���� �����ݴϴ�.
		EOpenDir ConnectDir = CheckDir(RandIdx);

		// ���� ������ �� �ִ� ���� �����Ѵٸ�?
		if (ConnectDir != EOpenDir::EOD_NONE)
		{
			// �ش� ����� �̾��ֵ��� �մϴ�.
			// * ���� ��
			DungeonMaps[RandIdx].OpenDir |= static_cast<uint8>(ConnectDir);

			// * ������ ��
			switch (ConnectDir)
			{
			case EOpenDir::EOD_LEFT:
				DungeonMaps[RandIdx - 1].OpenDir |= static_cast<uint8>(EOpenDir::EOD_RIGHT);
				break;

			case EOpenDir::EOD_RIGHT:
				DungeonMaps[RandIdx + 1].OpenDir |= static_cast<uint8>(EOpenDir::EOD_LEFT);
				break;

			case EOpenDir::EOD_UP:
				DungeonMaps[RandIdx - Width].OpenDir |= static_cast<uint8>(EOpenDir::EOD_DOWN);
				break;

			case EOpenDir::EOD_DOWN:
				DungeonMaps[RandIdx + Width].OpenDir |= static_cast<uint8>(EOpenDir::EOD_UP);
				break;
			}
		}

		// �ش� ���� �����¿츦 üũ�Ͽ� �湮���� ���� ����� �迭�� �߰��մϴ�.
		if (DungeonMaps.IsValidIndex(RandIdx - Width) && DungeonMaps[RandIdx - Width].bIsVisited == false) TempRooms.Add(RandIdx - Width);
		if (DungeonMaps.IsValidIndex(RandIdx + Width) && DungeonMaps[RandIdx + Width].bIsVisited == false) TempRooms.Add(RandIdx + Width);
		if ((RandIdx / Width) == ((RandIdx + 1) / Width) && DungeonMaps.IsValidIndex(RandIdx + 1) && DungeonMaps[RandIdx + 1].bIsVisited == false) TempRooms.Add(RandIdx + 1);
		if ((RandIdx / Width) == ((RandIdx - 1) / Width) && DungeonMaps.IsValidIndex(RandIdx - 1) && DungeonMaps[RandIdx - 1].bIsVisited == false) TempRooms.Add(RandIdx - 1);
	}
}

void UDungeonGeneratorComponent::SpawnMaze()
{
	// �� ���� ũ�� 4000 * 4000

	// ��� ���� ������ ��ȸ�ϸ� �˸��� ��ġ�� �����մϴ�.
	for (int32 y = 0; y < Height; y++)
	{
		for (int32 x = 0; x < Width; x++)
		{
			// ���� �ε����� �ùٸ��� Ȯ��
			if (DungeonMaps.IsValidIndex((y * Width) + x))
			{
				// ���� ��ġ ���ϱ�
				FTransform SpawnTransform;

				SpawnTransform.SetLocation(FVector(-RoomSize * y, RoomSize * x, 0.0f));

				// ������ Ŭ���� �������� ���ϱ�
				int32 RandIdx = FMath::RandRange(0, RoomClassArray.Num() - 1);

				// �ش� ��ġ�� �����ϱ�

				ARoomBase* Room;
				// * ���� ��
				if ((y * Width) + x == 0)
				{
					Room = GetWorld()->SpawnActorDeferred<ARoomBase>(StartRoomClass, SpawnTransform);
				}
				// * ������
				else if ((y * Width) + x == Width * Height - 1)
				{
					Room = GetWorld()->SpawnActorDeferred<ARoomBase>(BossRoomClass, SpawnTransform);
				}
				// * �Ϲ� ��
				else
				{
					Room = GetWorld()->SpawnActorDeferred<ARoomBase>(RoomClassArray[RandIdx], SpawnTransform);
				}
				
				if (Room)
				{
					Room->InitRoom(DungeonMaps[(y * Width) + x].OpenDir, DungeonMaps[(y * Width) + x].RoomNum);
					Room->FinishSpawning(SpawnTransform);
				}	
			}
		}
	}
}

EOpenDir UDungeonGeneratorComponent::CheckDir(int32 CurrentIdx)
{
	// ���⼭�� ���� �ε������� ������ �̹� �湮�� �� �ϳ��� �������� ã�Ƽ� ��ȯ�ؾ� �մϴ�.
	// * �����¿� üũ�� �����մϴ�.
	TArray<EOpenDir> Indexs;

	// ����
	if (DungeonMaps.IsValidIndex(CurrentIdx - Width) && DungeonMaps[CurrentIdx - Width].bIsVisited)
	{ 
		Indexs.Add(EOpenDir::EOD_UP);
	}
	// �Ʒ���
	if (DungeonMaps.IsValidIndex(CurrentIdx + Width) && DungeonMaps[CurrentIdx + Width].bIsVisited)
	{
		Indexs.Add(EOpenDir::EOD_DOWN);
	}
	// ������
	if ((CurrentIdx / Width) == ((CurrentIdx + 1) / Width) && DungeonMaps.IsValidIndex(CurrentIdx + 1) && DungeonMaps[CurrentIdx + 1].bIsVisited)
	{
		Indexs.Add(EOpenDir::EOD_RIGHT);
	}
	// ����
	if ((CurrentIdx / Width) == ((CurrentIdx - 1) / Width) && DungeonMaps.IsValidIndex(CurrentIdx - 1) && DungeonMaps[CurrentIdx - 1].bIsVisited)
	{
		Indexs.Add(EOpenDir::EOD_LEFT);
	}

	// ���� ã�� �ε����� �����Ѵٸ�?
	if (Indexs.Num())
	{
		// �������� ���� �����մϴ�.
		return Indexs[FMath::RandRange(0, Indexs.Num() - 1)];
	}
	else
	{
		// ��� ������ �Ұ����� ��� -1�� ��ȯ�մϴ�.
		return EOpenDir::EOD_NONE;
	}
}


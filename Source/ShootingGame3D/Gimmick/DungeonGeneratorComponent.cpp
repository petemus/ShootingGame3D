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
	// 1. Height * Width 만큼 배열 초기화
	//    = Room 숫자 및 열리는 위치 초기화
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

	// 다음으로 체크 가능한 방 목록
	TArray<int32> TempRooms;

	// 시작 위치(0,0)의 아래, 오른쪽 체크해서 탐색 가능한 방 배열에 추가하기
	DungeonMaps[0].bIsVisited = true;
	TempRooms.Add(Width);
	TempRooms.Add(1);

	int32 CheckCnt = 1;

	// 2. 모든 방이 체크가 될 때까지 반복해서 미로 생성하기
	while (CheckCnt != DungeonMaps.Num())
	{
		// 랜덤한 방 뽑기
		int32 RandIdx = TempRooms[FMath::RandRange(0, TempRooms.Num() - 1)];

		// 해당 방 방문처리
		DungeonMaps[RandIdx].bIsVisited = true;
		// 카운트 증가
		CheckCnt++;
		// 해당 방 임시 방 목록에서 제거
		TempRooms.Remove(RandIdx);

		// 해당 방으로부터 상하좌우 체크하여 연결해줄 방을 구해줍니다.
		EOpenDir ConnectDir = CheckDir(RandIdx);

		// 만약 연결할 수 있는 방이 존재한다면?
		if (ConnectDir != EOpenDir::EOD_NONE)
		{
			// 해당 방향과 이어주도록 합니다.
			// * 현재 방
			DungeonMaps[RandIdx].OpenDir |= static_cast<uint8>(ConnectDir);

			// * 연결한 방
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

		// 해당 방의 상하좌우를 체크하여 방문되지 않은 방들을 배열에 추가합니다.
		if (DungeonMaps.IsValidIndex(RandIdx - Width) && DungeonMaps[RandIdx - Width].bIsVisited == false) TempRooms.Add(RandIdx - Width);
		if (DungeonMaps.IsValidIndex(RandIdx + Width) && DungeonMaps[RandIdx + Width].bIsVisited == false) TempRooms.Add(RandIdx + Width);
		if ((RandIdx / Width) == ((RandIdx + 1) / Width) && DungeonMaps.IsValidIndex(RandIdx + 1) && DungeonMaps[RandIdx + 1].bIsVisited == false) TempRooms.Add(RandIdx + 1);
		if ((RandIdx / Width) == ((RandIdx - 1) / Width) && DungeonMaps.IsValidIndex(RandIdx - 1) && DungeonMaps[RandIdx - 1].bIsVisited == false) TempRooms.Add(RandIdx - 1);
	}
}

void UDungeonGeneratorComponent::SpawnMaze()
{
	// 한 방의 크기 4000 * 4000

	// 모든 방의 정보를 순회하며 알맞은 위치에 스폰합니다.
	for (int32 y = 0; y < Height; y++)
	{
		for (int32 x = 0; x < Width; x++)
		{
			// 현재 인덱스가 올바른지 확인
			if (DungeonMaps.IsValidIndex((y * Width) + x))
			{
				// 스폰 위치 구하기
				FTransform SpawnTransform;

				SpawnTransform.SetLocation(FVector(-RoomSize * y, RoomSize * x, 0.0f));

				// 스폰할 클래스 랜덤으로 구하기
				int32 RandIdx = FMath::RandRange(0, RoomClassArray.Num() - 1);

				// 해당 위치에 스폰하기

				ARoomBase* Room;
				// * 시작 방
				if ((y * Width) + x == 0)
				{
					Room = GetWorld()->SpawnActorDeferred<ARoomBase>(StartRoomClass, SpawnTransform);
				}
				// * 보스방
				else if ((y * Width) + x == Width * Height - 1)
				{
					Room = GetWorld()->SpawnActorDeferred<ARoomBase>(BossRoomClass, SpawnTransform);
				}
				// * 일반 방
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
	// 여기서는 현재 인덱스에서 인접한 이미 방문한 방 하나를 랜덤으로 찾아서 반환해야 합니다.
	// * 상하좌우 체크를 진행합니다.
	TArray<EOpenDir> Indexs;

	// 위쪽
	if (DungeonMaps.IsValidIndex(CurrentIdx - Width) && DungeonMaps[CurrentIdx - Width].bIsVisited)
	{ 
		Indexs.Add(EOpenDir::EOD_UP);
	}
	// 아래쪽
	if (DungeonMaps.IsValidIndex(CurrentIdx + Width) && DungeonMaps[CurrentIdx + Width].bIsVisited)
	{
		Indexs.Add(EOpenDir::EOD_DOWN);
	}
	// 오른쪽
	if ((CurrentIdx / Width) == ((CurrentIdx + 1) / Width) && DungeonMaps.IsValidIndex(CurrentIdx + 1) && DungeonMaps[CurrentIdx + 1].bIsVisited)
	{
		Indexs.Add(EOpenDir::EOD_RIGHT);
	}
	// 왼쪽
	if ((CurrentIdx / Width) == ((CurrentIdx - 1) / Width) && DungeonMaps.IsValidIndex(CurrentIdx - 1) && DungeonMaps[CurrentIdx - 1].bIsVisited)
	{
		Indexs.Add(EOpenDir::EOD_LEFT);
	}

	// 만약 찾은 인덱스가 존재한다면?
	if (Indexs.Num())
	{
		// 랜덤으로 값을 추출합니다.
		return Indexs[FMath::RandRange(0, Indexs.Num() - 1)];
	}
	else
	{
		// 모두 연결이 불가능한 경우 -1을 반환합니다.
		return EOpenDir::EOD_NONE;
	}
}


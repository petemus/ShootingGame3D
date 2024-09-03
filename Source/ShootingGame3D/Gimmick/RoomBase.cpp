// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomBase.h"
#include "ShootingGame3D/Player/PlayerCharacter.h"
#include "ShootingGame3D/Enemy/Enemy.h"
#include "ShootingGame3D/Public/ShootingGameModeBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ARoomBase::ARoomBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Sub Object
	CreateMeshComponent();

	// Set Transform
	InitRoomTransform();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARoomBase::OnComponentBeginOverlap);

	// Member Init
	CurrentRoomState = ERoomState::RS_None;
	SpawnCount = 0;
}

void ARoomBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomBase::InitRoom(uint8 OpenDirFlag, int32 RoomCount)
{
	// 현재 방의 넘버 저장
	RoomNum = RoomCount;
	RoomOpenFlag = OpenDirFlag;

	// 연결된 Map 구조에 따라 초기화
	if (OpenDirFlag & static_cast<uint8>(EOpenDir::EOD_LEFT)) SetDoor(EOpenDir::EOD_LEFT);
	if (OpenDirFlag & static_cast<uint8>(EOpenDir::EOD_RIGHT)) SetDoor(EOpenDir::EOD_RIGHT);
	if (OpenDirFlag & static_cast<uint8>(EOpenDir::EOD_UP)) SetDoor(EOpenDir::EOD_UP);
	if (OpenDirFlag & static_cast<uint8>(EOpenDir::EOD_DOWN)) SetDoor(EOpenDir::EOD_DOWN);

	// 초기 상태 변경
	SetState(ERoomState::RS_Ready);
}

void ARoomBase::SetDoor(EOpenDir OpenDir)
{
	switch (OpenDir)
	{
	case EOpenDir::EOD_LEFT:
		Left_Door->SetVisibility(true);
		Left_Wall->SetVisibility(false);
		Left_Wall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EOpenDir::EOD_RIGHT:
		Right_Door->SetVisibility(true);
		Right_Wall->SetVisibility(false);
		Right_Wall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EOpenDir::EOD_UP:
		Up_Door->SetVisibility(true);
		Up_Wall->SetVisibility(false);
		Up_Wall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case EOpenDir::EOD_DOWN:
		Down_Door->SetVisibility(true);
		Down_Wall->SetVisibility(false);
		Down_Wall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	}
}

void ARoomBase::OpenDoor(bool bIsOpen)
{
	if (bIsOpen)
	{
		if (Left_Door->IsVisible())
		{
			FVector NewLocation = Left_Door->GetRelativeLocation() - FVector(0.0f, 0.0f, 1800.0f);
			Left_Door->SetRelativeLocation(NewLocation);
		}

		if (Right_Door->IsVisible())
		{
			FVector NewLocation = Right_Door->GetRelativeLocation() - FVector(0.0f, 0.0f, 1800.0f);
			Right_Door->SetRelativeLocation(NewLocation);
		}

		if (Up_Door->IsVisible())
		{
			FVector NewLocation = Up_Door->GetRelativeLocation() - FVector(0.0f, 0.0f, 1800.0f);
			Up_Door->SetRelativeLocation(NewLocation);
		}

		if (Down_Door->IsVisible())
		{
			FVector NewLocation = Down_Door->GetRelativeLocation() - FVector(0.0f, 0.0f, 1800.0f);
			Down_Door->SetRelativeLocation(NewLocation);
		}
	}
	else
	{
		if (Left_Door->IsVisible())
		{
			FVector NewLocation = Left_Door->GetRelativeLocation() + FVector(0.0f, 0.0f, 1800.0f);
			Left_Door->SetRelativeLocation(NewLocation);
		}

		if (Right_Door->IsVisible())
		{
			FVector NewLocation = Right_Door->GetRelativeLocation() + FVector(0.0f, 0.0f, 1800.0f);
			Right_Door->SetRelativeLocation(NewLocation);
		}

		if (Up_Door->IsVisible())
		{
			FVector NewLocation = Up_Door->GetRelativeLocation() + FVector(0.0f, 0.0f, 1800.0f);
			Up_Door->SetRelativeLocation(NewLocation);
		}

		if (Down_Door->IsVisible())
		{
			FVector NewLocation = Down_Door->GetRelativeLocation() + FVector(0.0f, 0.0f, 1800.0f);
			Down_Door->SetRelativeLocation(NewLocation);
		}
	}
}

void ARoomBase::SpawnMonster()
{
	UWorld* World = GetWorld();
	if (!World) return;

	for (int32 i = 0; i < MonsterSpawnInfos.Num(); i++)
	{
		FVector SpawnLocation = MonsterSpawnInfos[i].SpawnRelativeLocation + GetActorLocation();
		FRotator SpawnRotation = MonsterSpawnInfos[i].SpawnRelativeRotation + GetActorRotation();

		AActor* Monster = World->SpawnActor<AEnemy>(MonsterSpawnInfos[i].MonsterClass, SpawnLocation, SpawnRotation);
		if (Monster)
		{
			// 스폰 후 Owner 설정 및 카운트 증가
			Monster->SetOwner(this);
			SpawnCount++;
		}
	}
}

void ARoomBase::DecreaseCount()
{
	SpawnCount--;

	if (SpawnCount == 0)
	{
		SetState(ERoomState::RS_End);
	}
}

void ARoomBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌한 액터가 플레이어라면?
	if (Cast<APlayerCharacter>(OtherActor))
	{
		// 현재 방의 상태가 Ready라면?
		if (CurrentRoomState == ERoomState::RS_Ready)
		{
			// 방의 상태 변경
			SetState(ERoomState::RS_Start);
		}
		
		AShootingGameModeBase* GM = Cast<AShootingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->MiniMapSet(RoomNum, RoomOpenFlag);
		}
	}
}

void ARoomBase::SetState(ERoomState InState)
{
	if (CurrentRoomState == InState) return;

	CurrentRoomState = InState;

	switch (CurrentRoomState)
	{
	case ERoomState::RS_Ready:
		ReadyRoom();
		break;

	case ERoomState::RS_Start:
		StartRoom();
		break;

	case ERoomState::RS_End:
		EndRoom();
		break;
	}
}

void ARoomBase::ReadyRoom()
{
	// 문 열기
	OpenDoor(true);
}

void ARoomBase::StartRoom()
{
	// 문 닫기
	OpenDoor(false);
	// 1초 후 몬스터 스폰 (타이머 이용)
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ARoomBase::SpawnMonster);

	SpawnTimerHandle.Invalidate();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, TimerDelegate, 2.0f, false);
}

void ARoomBase::EndRoom()
{
	// 문 열기
	OpenDoor(true);
}

void ARoomBase::CreateMeshComponent()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	BoxComp->SetBoxExtent(FVector(1900.0f, 1900.0f, 100.0f));
	BoxComp->SetCollisionProfileName(TEXT("Room"));

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>FloorMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Dungeon_Bundle/DepthsBelow/Geometry/Floor/SM_FloorTiles02.SM_FloorTiles02'"));
	if (FloorMeshRef.Object)
	{
		FloorMesh->SetStaticMesh(FloorMeshRef.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh>WallBaseMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Assets/DungeonTomb/Meshes/Construction/Wall/SM_Wall_Base_01_Flat_01_Straight_600_400_A.SM_Wall_Base_01_Flat_01_Straight_600_400_A'"));
	for (int i = 0; i < 8; i++)
	{
		FName DisplayName = *FString::Printf(TEXT("Wall%d"), i);
		WallMeshes.Add(CreateDefaultSubobject<UStaticMeshComponent>(DisplayName));
		WallMeshes[i]->SetupAttachment(RootComponent);

		if (WallBaseMeshRef.Object)
		{
			WallMeshes[i]->SetStaticMesh(WallBaseMeshRef.Object);
		}
	}

	Left_Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left_Door"));
	Left_Door->SetupAttachment(RootComponent);
	Right_Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right_Door"));
	Right_Door->SetupAttachment(RootComponent);
	Up_Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Up_Door"));
	Up_Door->SetupAttachment(RootComponent);
	Down_Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down_Door"));
	Down_Door->SetupAttachment(RootComponent);

	Left_Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left_Wall"));
	Left_Wall->SetupAttachment(RootComponent);
	Right_Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right_Wall"));
	Right_Wall->SetupAttachment(RootComponent);
	Up_Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Up_Wall"));
	Up_Wall->SetupAttachment(RootComponent);
	Down_Wall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Down_Wall"));
	Down_Wall->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>HiddenWallMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (HiddenWallMeshRef.Object)
	{
		Left_Wall->SetStaticMesh(HiddenWallMeshRef.Object);
		Right_Wall->SetStaticMesh(HiddenWallMeshRef.Object);
		Up_Wall->SetStaticMesh(HiddenWallMeshRef.Object);
		Down_Wall->SetStaticMesh(HiddenWallMeshRef.Object);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh>HiddenDoorMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Dungeon_Bundle/DepthsBelow/Geometry/Props/Fence/SM_FenceDoor01.SM_FenceDoor01'"));
	if (HiddenDoorMeshRef.Object)
	{
		Left_Door->SetStaticMesh(HiddenDoorMeshRef.Object);
		Right_Door->SetStaticMesh(HiddenDoorMeshRef.Object);
		Up_Door->SetStaticMesh(HiddenDoorMeshRef.Object);
		Down_Door->SetStaticMesh(HiddenDoorMeshRef.Object);
	}
}

void ARoomBase::InitRoomTransform()
{
	FloorMesh->SetRelativeScale3D(FVector(10.0f, 10.0f, 1.0f));

	Left_Wall->SetRelativeLocationAndRotation(FVector(0.0f, -2000.0f, 850.0f), FRotator(0.0f, 90.0f, 0.0f));
	Left_Wall->SetRelativeScale3D(FVector(1.0f, 8.0f, 18.0f));
	Right_Wall->SetRelativeLocationAndRotation(FVector(0.0f, 2000.0f, 850.0f), FRotator(0.0f, -90.0f, 0.0f));
	Right_Wall->SetRelativeScale3D(FVector(1.0f, 8.0f, 18.0f));
	Up_Wall->SetRelativeLocation(FVector(2000.0f, 0.0f, 850.0f));
	Up_Wall->SetRelativeScale3D(FVector(1.0f, 8.0f, 18.0f));
	Down_Wall->SetRelativeLocation(FVector(-2000.0f, 0.0f, 850.0f));
	Down_Wall->SetRelativeScale3D(FVector(1.0f, 8.0f, 18.0f));

	Left_Wall->SetVisibility(true);
	Right_Wall->SetVisibility(true);
	Up_Wall->SetVisibility(true);
	Down_Wall->SetVisibility(true);

	Left_Door->SetRelativeLocation(FVector(-425.0f, -2000.0f, -50.0f));
	Left_Door->SetRelativeScale3D(FVector(5.0f, 1.0f, 5.0f));
	Right_Door->SetRelativeLocationAndRotation(FVector(425.0f, 2000.0f, -50.0f), FRotator(0.0f, 180.0f, 0.0f));
	Right_Door->SetRelativeScale3D(FVector(5.0f, 1.0f, 5.0f));
	Up_Door->SetRelativeLocationAndRotation(FVector(2000.0f, -425.0f, -50.0f), FRotator(0.0f, 90.0f, 0.0f));
	Up_Door->SetRelativeScale3D(FVector(5.0f, 1.0f, 5.0f));
	Down_Door->SetRelativeLocationAndRotation(FVector(-2000.0f, 425.0f, -50.0f), FRotator(0.0f, -90.0f, 0.0f));
	Down_Door->SetRelativeScale3D(FVector(5.0f, 1.0f, 5.0f));

	Left_Door->SetVisibility(false);
	Right_Door->SetVisibility(false);
	Up_Door->SetVisibility(false);
	Down_Door->SetVisibility(false);

	WallMeshes[0]->SetRelativeLocationAndRotation(FVector(2000.0f, 400.0f, -40.0f), FRotator(0.0f, 90.0f, 0.0f));
	WallMeshes[0]->SetRelativeScale3D(FVector(4.0f, 1.0f, 3.0f));
	WallMeshes[1]->SetRelativeLocationAndRotation(FVector(2000.0f, -2000.0f, -40.0f), FRotator(0.0f, 90.0f, 0.0f));
	WallMeshes[1]->SetRelativeScale3D(FVector(4.0f, 1.0f, 3.0f));
	WallMeshes[2]->SetRelativeLocationAndRotation(FVector(2000.0f, 2000.0f, -40.0f), FRotator(0.0f, 180.0f, 0.0f));
	WallMeshes[2]->SetRelativeScale3D(FVector(4.0f, 1.0f, 3.0f));
	WallMeshes[3]->SetRelativeLocationAndRotation(FVector(-400.0f, 2000.0f, -40.0f), FRotator(0.0f, 180.0f, 0.0f));
	WallMeshes[3]->SetRelativeScale3D(FVector(4.0f, 1.0f, 3.0f));
	WallMeshes[4]->SetRelativeLocationAndRotation(FVector(-2000.0f, -400.0f, -40.0f), FRotator(0.0f, -90.0f, 0.0f));
	WallMeshes[4]->SetRelativeScale3D(FVector(4.0f, 1.0f, 3.0f));
	WallMeshes[5]->SetRelativeLocationAndRotation(FVector(-2000.0f, 2000.0f, -40.0f), FRotator(0.0f, -90.0f, 0.0f));
	WallMeshes[5]->SetRelativeScale3D(FVector(4.0f, 1.0f, 3.0f));
	WallMeshes[6]->SetRelativeLocation(FVector(-2000.0f, -2000.0f, -40.0f));
	WallMeshes[6]->SetRelativeScale3D(FVector(4.0f, 1.0f, 3.0f));
	WallMeshes[7]->SetRelativeLocation(FVector(400.0f, -2000.0f, -40.0f));
	WallMeshes[7]->SetRelativeScale3D(FVector(4.0f, 1.0f, 3.0f));
}

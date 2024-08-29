// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomBase.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

ARoomBase::ARoomBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Sub Object
	CreateMeshComponent();

	// Set Transform
	InitRoomTransform();
}

void ARoomBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomBase::CreateMeshComponent()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

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

	Left_Wall->SetVisibility(false);
	Right_Wall->SetVisibility(false);
	Up_Wall->SetVisibility(false);
	Down_Wall->SetVisibility(false);

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


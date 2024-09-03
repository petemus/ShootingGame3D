// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomInterface.h"
#include "RoomEnums.h"
#include "RoomBase.generated.h"

UENUM(BlueprintType)
enum class ERoomState : uint8
{
	RS_None,	// None
	RS_Ready,	// Init
	RS_Start,	// Player Enter
	RS_End,		// Room Clear
};

USTRUCT(BlueprintType)
struct FMonsterSpawnInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemy> MonsterClass;

	UPROPERTY(EditAnywhere)
	FVector SpawnRelativeLocation;

	UPROPERTY(EditAnywhere)
	FRotator SpawnRelativeRotation;
};

UCLASS()
class SHOOTINGGAME3D_API ARoomBase : public AActor, public IRoomInterface
{
	GENERATED_BODY()
	
public:
	ARoomBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

// Init
public:
	void InitRoom(uint8 OpenDirFlag, int32 RoomCount);

// Create
private:
	void CreateMeshComponent();
	void InitRoomTransform();
	void SetDoor(EOpenDir OpenDir);

// Collision
protected:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

// RoomState
protected:
	// Current State
	ERoomState CurrentRoomState;

	// Change Room State
	void SetState(ERoomState InState);

	// Init Room (Start : Open Door)
	void ReadyRoom();
	// Start Room (Player Enter : Close Door, Spawn Monster)
	void StartRoom();
	// End Room (Room Clear : Open Door)
	virtual void EndRoom();

// Game Logic
private:
	void OpenDoor(bool bIsOpen);
	void SpawnMonster();

public:
	virtual void DecreaseCount() override;
	FORCEINLINE virtual int32 GetRoomNum() override { return RoomNum; }

// Components
private:
	// Collision
	UPROPERTY(VisibleAnywhere, Category = "Room")
	TObjectPtr<class UBoxComponent> BoxComp;

	// Mesh
	UPROPERTY(VisibleAnywhere, Category = "Room")
	TObjectPtr<class UStaticMeshComponent> FloorMesh;

	UPROPERTY(VisibleAnywhere, Category = "Room")
	TArray<TObjectPtr<class UStaticMeshComponent>> WallMeshes;

	UPROPERTY(VisibleAnywhere, Category = "Room")
	TObjectPtr<class UStaticMeshComponent> Left_Door;
	UPROPERTY(VisibleAnywhere, Category = "Room")
	TObjectPtr<class UStaticMeshComponent> Left_Wall;

	UPROPERTY(VisibleAnywhere, Category = "Room")
	TObjectPtr<class UStaticMeshComponent> Right_Door;
	UPROPERTY(VisibleAnywhere, Category = "Room")
	TObjectPtr<class UStaticMeshComponent> Right_Wall;

	UPROPERTY(VisibleAnywhere, Category = "Room")
	TObjectPtr<class UStaticMeshComponent> Up_Door;
	UPROPERTY(VisibleAnywhere, Category = "Room")
	TObjectPtr<class UStaticMeshComponent> Up_Wall;

	UPROPERTY(VisibleAnywhere, Category = "Room")
	TObjectPtr<class UStaticMeshComponent> Down_Door;
	UPROPERTY(VisibleAnywhere, Category = "Room")
	TObjectPtr<class UStaticMeshComponent> Down_Wall;

private:
	UPROPERTY(EditAnywhere, Category = "Room")
	TArray<FMonsterSpawnInfo> MonsterSpawnInfos;

	FTimerHandle SpawnTimerHandle;

	int32 SpawnCount;

	UPROPERTY(VisibleAnywhere)
	int32 RoomNum;

	UPROPERTY(VisibleAnywhere)
	uint8 RoomOpenFlag;
};

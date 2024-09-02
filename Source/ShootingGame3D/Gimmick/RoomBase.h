// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomEnums.h"
#include "RoomBase.generated.h"

UCLASS()
class SHOOTINGGAME3D_API ARoomBase : public AActor
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
	void InitRoom(uint8 OpenDirFlag);

// Create
private:
	void CreateMeshComponent();
	void InitRoomTransform();
	void SetDoor(EOpenDir OpenDir);
	void OpenDoor();

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
};

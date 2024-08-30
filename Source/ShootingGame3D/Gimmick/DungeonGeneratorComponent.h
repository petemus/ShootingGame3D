// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RoomInfo.h"
#include "DungeonGeneratorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTINGGAME3D_API UDungeonGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDungeonGeneratorComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void GenerateMaze();
	void SpawnMaze();
	EOpenDir CheckDir(int32 CurrentIdx);

// Member Variable
private:
	UPROPERTY(EditAnywhere, Category = "Dungeon")
	int32 Width;

	UPROPERTY(EditAnywhere, Category = "Dungeon")
	int32 Height;

	UPROPERTY(VisibleAnywhere, Category = "Dungeon")
	TArray<FRoomInfo> DungeonMaps;

	UPROPERTY(EditAnywhere, Category = "Dungeon")
	TArray<TSubclassOf<class ARoomBase>> RoomClassArray;
};

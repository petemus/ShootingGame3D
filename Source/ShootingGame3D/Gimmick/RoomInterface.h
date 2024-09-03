// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RoomInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URoomInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHOOTINGGAME3D_API IRoomInterface
{
	GENERATED_BODY()

public:
	virtual void DecreaseCount() = 0;
	virtual int32 GetRoomNum() = 0;
};

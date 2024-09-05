// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomBase.h"
#include "BossRoom.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME3D_API ABossRoom : public ARoomBase
{
	GENERATED_BODY()
	
public:
	ABossRoom();

public:
	virtual void Tick(float DeltaTime) override;

// RoomState
protected:
	// Start Room (BossSound Play)
	virtual void StartRoom() override;
	// End Room (Room Clear : Open Door)
	virtual void EndRoom() override;
};

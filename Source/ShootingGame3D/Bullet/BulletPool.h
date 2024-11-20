// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletPool.generated.h"

UCLASS()
class SHOOTINGGAME3D_API ABulletPool : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABulletPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

// Bullet 가져오기
public:
	TUniquePtr<ABullet> GetBullet();
	void ReturnBullet(TUniquePtr<ABullet> Bullet);

private:
	void InitializePool();
private:
	TArray<TUniquePtr<ABullet>> BulletPool;
	int32 PoolSize = 100;

	int32 AddBulletSize = 10;


};

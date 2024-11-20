// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPool.h"

#include "Bullet.h"


// Sets default values
ABulletPool::ABulletPool()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABulletPool::BeginPlay()
{
	Super::BeginPlay();
	InitializePool();
}

// Called every frame
void ABulletPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TUniquePtr<ABullet> ABulletPool::GetBullet()
{
	bool isAllActive = true;
	for (int32 i = 0; i < BulletPool.Num(); ++i)
	{
		if (!BulletPool[i]->IsActive())
		{
			isAllActive = false;
			return MoveTemp(BulletPool[i]);
		}
	}

	// 만약 모든 bullet이 Active 상태인 경우 Bullet 추가
	if(isAllActive)
	{
		for (int32 i = 0; i < AddBulletSize; ++i)
		{
			TUniquePtr<ABullet> Bullet = MakeUnique<ABullet>();
			Bullet->Deactivate();
			BulletPool.Add(MoveTemp(Bullet));
		}
		return MoveTemp(BulletPool[BulletPool.Num() - 1]);
	}

}

void ABulletPool::ReturnBullet(TUniquePtr<ABullet> Bullet)
{
	Bullet->Deactivate();
	BulletPool.Add(MoveTemp(Bullet));
}



void ABulletPool::InitializePool()
{
	for (int32 i = 0; i < PoolSize; ++i)
	{
		TUniquePtr<ABullet> Bullet = MakeUnique<ABullet>();
		Bullet->Deactivate();
		BulletPool.Add(MoveTemp(Bullet));
	}
}


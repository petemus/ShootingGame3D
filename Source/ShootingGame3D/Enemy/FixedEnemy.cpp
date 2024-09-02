// Fill out your copyright notice in the Description page of Project Settings.


#include "FixedEnemy.h"

void AFixedEnemy::BeginPlay()
{
	Super::BeginPlay();
	FVector EnemyPos = GetActorLocation();
	FVector InitPos = EnemyPos + (GetActorForwardVector() * 100.f);

	FVector Offset = InitPos - EnemyPos;

	for (int i = 0; i < 8; ++i)
	{
		float Angle = 45.f * i;
		FRotator Rot = FRotator(0.f, Angle, 0.f);

		FVector RotVec = Rot.RotateVector(Offset);
		FVector NewPos = EnemyPos + RotVec;

		FirePoses.AddUnique(NewPos);
	}
}

void AFixedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFixedEnemy::Move(float DeltaTime)
{
}

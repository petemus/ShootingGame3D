// Fill out your copyright notice in the Description page of Project Settings.


#include "InDirectEnemyBullet.h"

void AInDirectEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

	StartPosition = GetActorLocation();
}

void AInDirectEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

void AInDirectEnemyBullet::Move(float Delta)
{
	float TimeRatio = (GetWorld()->GetTimeSeconds() - StartTime) / TotalTime;

	if (TimeRatio >= 1.f)
	{
		TimeRatio = 1.f;
	}

	FVector L1 = FMath::Lerp(StartPosition, HighPosition, TimeRatio);
	FVector L2 = FMath::Lerp(HighPosition, TargetPosition, TimeRatio);

	FVector CurrentPosition = FMath::Lerp(L1, L2, TimeRatio);

	SetActorLocation(CurrentPosition);

	if (TimeRatio >= 1.f)
	{
		bool bWasDestroyed = Destroy(); 				
	}
}

void AInDirectEnemyBullet::InitialVar(FVector Target, FVector High, float Time)
{
	TargetPosition = Target;
	HighPosition = High;
	TotalTime = Time;
	StartTime = GetWorld()->GetTimeSeconds();

}

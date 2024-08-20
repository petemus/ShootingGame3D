// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectEnemyBullet.h"

void ADirectEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

}

void ADirectEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

void ADirectEnemyBullet::Move(float Delta)
{
	FVector CurrentForward = GetActorForwardVector();
	CurrentForward = CurrentForward * MoveSpeed * Delta;

	FVector NextLocation = CurrentForward + GetActorLocation();

	SetActorLocation(NextLocation);
}

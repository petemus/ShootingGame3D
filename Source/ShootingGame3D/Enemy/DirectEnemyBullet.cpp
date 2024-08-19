// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectEnemyBullet.h"

void ADirectEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

	if (TargetObj)
	{
		FVector VecToRo = TargetObj->GetActorLocation() - GetActorLocation();

		VecToRo = VecToRo.GetSafeNormal();

		FRotator Rot = VecToRo.Rotation();

		FQuat Qut = Rot.Quaternion();

		SetActorRotation(Qut);
	}
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

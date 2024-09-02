// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemy.h"
#include "GameFramework/Character.h"

ANormalEnemy::ANormalEnemy()
{
}

void ANormalEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMove)
	{
		Move(DeltaTime);
	}
}


void ANormalEnemy::Move(float DeltaTime)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController) return;

	APawn* Pawn = PlayerController->GetPawn();

	if (!Pawn) return;

	
	FVector PlayerToMe = (Pawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	PlayerToMe = PlayerToMe * DeltaTime * MoveSpeed;

	FVector NewPosition = FVector((PlayerToMe + GetActorLocation()).X, (PlayerToMe + GetActorLocation()).Y, GetActorLocation().Z);

	SetActorLocation(NewPosition);

	FRotator Rot = FRotator(GetActorRotation().Pitch, PlayerToMe.Rotation().Yaw, GetActorRotation().Roll);
	FQuat Dir = Rot.Quaternion();

	SetActorRotation(Dir);
}

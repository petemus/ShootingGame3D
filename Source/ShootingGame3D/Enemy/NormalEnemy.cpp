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

	Move(DeltaTime);
}


void ANormalEnemy::Move(float DeltaTime)
{
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	if (!Controller) return;

	APawn* Pawn = Controller->GetPawn();

	if (!Pawn) return;

	ACharacter* Player = Cast<ACharacter>(Pawn);

	if (!Player) return;

	FVector PlayerToMe = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	PlayerToMe = PlayerToMe * DeltaTime * MoveSpeed;

	FVector NewPosition = FVector((PlayerToMe + GetActorLocation()).X, (PlayerToMe + GetActorLocation()).Y, GetActorLocation().Z);

	SetActorLocation(NewPosition);

	FRotator Rot = FRotator(GetActorRotation().Pitch, PlayerToMe.Rotation().Yaw, GetActorRotation().Roll);
	FQuat Dir = Rot.Quaternion();

	SetActorRotation(Dir);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolEnemy.h"

APatrolEnemy::APatrolEnemy()
{
}

void APatrolEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void APatrolEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    Move(DeltaTime);
}

void APatrolEnemy::Attack()
{

}

void APatrolEnemy::Move(float DeltaTime)
{
    
    FVector CurrentForward = GetActorForwardVector().GetSafeNormal();
    CurrentForward = CurrentForward * MoveSpeed * DeltaTime;

    FVector NextLocation = CurrentForward + GetActorLocation();

    SetActorLocation(NextLocation);

    float RotationSpeed = 60.0f;
    FRotator CurrentRotation = GetActorRotation();
    CurrentRotation.Yaw += RotationSpeed * DeltaTime;


    if (CurrentRotation.Yaw > 360.0f || CurrentRotation.Yaw < -360.0f)
    {
        // 나머지 연산
        CurrentRotation.Yaw = FMath::Fmod(CurrentRotation.Yaw, 360.0f);
    }

    SetActorRotation(CurrentRotation);
}

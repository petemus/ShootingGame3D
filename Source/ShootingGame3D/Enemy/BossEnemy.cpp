// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"

#include "DirectEnemyBullet.h"


// Sets default values
ABossEnemy::ABossEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	
}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();

	FVector EnemyPos = GetActorLocation();
	FVector InitPos = EnemyPos + (GetActorForwardVector() * 100.f);

	FVector Offset = InitPos - EnemyPos;

	for (int i = 0; i < 24; ++i)
	{
		float Angle = 15.f * i;
		FRotator Rot = FRotator(0.f, Angle, 0.f);

		FVector RotVec = Rot.RotateVector(Offset);
		FVector NewPos = EnemyPos + RotVec;

		FirePoses.AddUnique(NewPos);
	}
	
	GetWorld()->GetTimerManager().SetTimer(Attack1Timer,this,  &ABossEnemy::Attack1, 0.5f, true);
}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossEnemy::Attack1()
{
	if(Attack1Inx < 6)
	{
		for(int i = 0; i < 4; ++i)
		{
			int32 idx = i * 6 + Attack1Inx;
			FVector NewVec = GetActorLocation() - FirePoses[idx];
			NewVec.Normalize();

			FRotator Rot = NewVec.Rotation();
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(FirePoses[idx]);
			SpawnTransform.SetRotation(Rot.Quaternion());

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			
			GetWorld()->SpawnActor<ADirectEnemyBullet>(Bullet, SpawnTransform, SpawnParams);	
		}
		Attack1Inx++;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(Attack1Timer);
		Attack1Inx = 0;
	}
	
	
}


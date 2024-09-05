// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalEnemy.h"
#include "GameFramework/Character.h"
#include "ShootingGame3D/Bullet/Bullet.h"

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
	if(bIsMove)
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
	else
	{
		KnockBackTime+= DeltaTime;
		if(KnockBackTime >= KnockBackMaxTime)
		{
			bIsMove = true;
			KnockBackTime = 0.f;
		}
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		if (!PlayerController) return;

		APawn* Pawn = PlayerController->GetPawn();

		if (!Pawn) return;
		
		FVector PlayerToMe = (Pawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		PlayerToMe = PlayerToMe * DeltaTime * MoveSpeed;
		
		FRotator Rot = FRotator(GetActorRotation().Pitch, PlayerToMe.Rotation().Yaw, GetActorRotation().Roll);
		FQuat Dir = Rot.Quaternion();

		SetActorRotation(Dir);

	}
	
}

void ANormalEnemy::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCapsuleOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor && OtherActor != this)
	{
		ABullet* Bult = Cast<ABullet>(OtherActor);
		if(Bult)
		{
			bIsMove = false;
			KnockBackVec = Bult->GetActorLocation();
		}
	}
}

void ANormalEnemy::KnockBack()
{
	
}

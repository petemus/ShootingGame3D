// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"

#include "DirectEnemyBullet.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShootingGame3D/InGameFunc/SplineActorComponent.h"
#include "ShootingGame3D/Player/PlayerCharacter.h"


// Sets default values
ABossEnemy::ABossEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	currentState = EEnemyState::Idle;

	SplineComp = CreateDefaultSubobject<USplineActorComponent>(TEXT("Spline Component"));
	SplineComp->SetOwnerActor(this);
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
	
	
}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateFirePosition();
	switch (currentState)
	{
	case EEnemyState::None:
		
		break;
	case EEnemyState::Idle:
		Idle();
		break;
	case EEnemyState::Chasing:
		Chase();
		break;
	case EEnemyState::Attack:
		Attack();
		break;
	case EEnemyState::Die:
		Die();
		break;
	}

}

void ABossEnemy::CalChasingTime()
{
	ChasingTime += GetWorld()->GetDeltaSeconds();
	if(ChasingTime >= ChasingMaxTime)
	{
		ChasingTime = 0.f;

		// 나중에 릴펙터링
		bCanAttack1 = true;
		
		currentState = EEnemyState::Attack;
	}
}

void ABossEnemy::Move(float DeltaTime)
{
	Super::Move(DeltaTime);

	
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!PlayerController) return;

	APawn* Pawn = PlayerController->GetPawn();

	if (!Pawn) return;

	
	FVector PlayerToMe = (Pawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	PlayerToMe = PlayerToMe * GetWorld()->GetDeltaSeconds() * MoveSpeed;

	FVector NewPosition = FVector((PlayerToMe + GetActorLocation()).X, (PlayerToMe + GetActorLocation()).Y, GetActorLocation().Z);

	SetActorLocation(NewPosition);

	FRotator Rot = FRotator(GetActorRotation().Pitch, PlayerToMe.Rotation().Yaw, GetActorRotation().Roll);
	FQuat Dir = Rot.Quaternion();

	SetActorRotation(Dir);

}

void ABossEnemy::SetDamaged(int32 Amount)
{
	
	
}

void ABossEnemy::AttackPattern1()
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
		Attack1Loop++;
		Attack1Inx = 0;
	}

	if(Attack1Loop>=3)
	{
		GetWorld()->GetTimerManager().ClearTimer(Attack1Timer);
		Attack1Loop = 0;
		Attack1Inx = 0;
		currentState = EEnemyState::Chasing;
	}

	
}

void ABossEnemy::AttackPattern2()
{
	
	ACharacter* PlayerC = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if(!PlayerC) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerC);
	
	UE_LOG(LogTemp, Warning, TEXT("%f"), PlayerCharacter->GetActorLocation().X);

	float midX = (GetActorLocation().X + PlayerCharacter->GetActorLocation().X) * 0.5f;
	float midY = (GetActorLocation().Y + PlayerCharacter->GetActorLocation().Y) * 0.5f;
	float midZ = FMath::RandRange(200.f,400.f);
	
	FVector MidVector = FVector(midX, midY, midZ);
	
	SplineComp->Init(GetActorLocation(), MidVector, PlayerCharacter->GetActorLocation());
	
	GetWorld()->GetTimerManager().SetTimer(SplineTimer, this, &ABossEnemy::SplineMove, 3.f, false, 3.f);
	
}


void ABossEnemy::UpdateFirePosition()
{
	FVector EnemyPos = GetActorLocation();
	FVector InitPos = EnemyPos + (GetActorForwardVector() * 100.f);

	FVector Offset = InitPos - EnemyPos;

	for (int i = 0; i < 24; ++i)
	{
		float Angle = 15.f * i;
		FRotator Rot = FRotator(0.f, Angle, 0.f);

		FVector RotVec = Rot.RotateVector(Offset);
		FVector NewPos = EnemyPos + RotVec;

		FirePoses[i] = NewPos;
	}
}

void ABossEnemy::SplineMove()
{
	
}

void ABossEnemy::Idle()
{
	currentState = EEnemyState::Chasing;
}

void ABossEnemy::Chase()
{
	float Delta = GetWorld()->GetTimeSeconds();
	CalChasingTime();
	Move(Delta);
}

void ABossEnemy::Attack()
{
	int32 RandomIdx = FMath::RandRange(0,2);
	RandomIdx = 1;
	switch (RandomIdx)
	{
		case 0:
			if(bCanAttack1)
			{
				GetWorld()->GetTimerManager().SetTimer(Attack1Timer, this, &ABossEnemy::AttackPattern1, 0.1f, true);
				bCanAttack1 = false;
			}
			break;
		case 1:
			// 2초동안 경로를 보여주는 함수
			if(bCanAttack1)
			{
				AttackPattern2();
				bCanAttack1 = false;
			}
			break;
		case 2:
			
			break;
	}
}

void ABossEnemy::Die()
{
	
}


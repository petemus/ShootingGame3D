// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"

#include "DirectEnemyBullet.h"
#include "InDirectEnemyBullet.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
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
	SplineComp->SplineComponent->SetupAttachment(GetRootComponent());

	SmallCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SmallCircle"));	
	SmallCircle->SetupAttachment(GetRootComponent());
	SmallCircle->SetCollisionProfileName(TEXT("NoCollision"));

	BigCircle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BigCircle"));
	BigCircle->SetupAttachment(GetRootComponent());
	BigCircle->SetCollisionProfileName(TEXT("NoCollision"));
	
	SmallCircleCol = CreateDefaultSubobject<USphereComponent>(TEXT("SmallCircleCol"));
	SmallCircleCol->SetupAttachment(GetRootComponent());
	SmallCircleCol->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	SmallCircleCol->SetCollisionResponseToChannel(ECC_EngineTraceChannel5, ECR_Ignore);
	SmallCircleCol->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::OnSmallCircleOverlap);
	
	BigCircleCol = CreateDefaultSubobject<USphereComponent>(TEXT("BigCircleCol"));
	BigCircleCol->SetupAttachment(GetRootComponent());
	BigCircleCol->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
	BigCircleCol->SetCollisionResponseToChannel(ECC_EngineTraceChannel5, ECR_Ignore);
	BigCircleCol->OnComponentBeginOverlap.AddDynamic(this, &ABossEnemy::ABossEnemy::OnBigCircleOverlap);
	
	//SmallCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SmallCircleDecal"));
	//SmallCircleDecal->SetupAttachment(GetRootComponent());
	//SmallCircleDecal->DecalSize = FVector(100.0f, 200.0f, 200.0f);
////
	//BigCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("BigCircleDecal"));
	//BigCircleDecal->SetupAttachment(GetRootComponent());
	//BigCircleDecal->DecalSize = FVector(200.0f, 400.0f, 400.0f);

		static ConstructorHelpers::FObjectFinder<UMaterial> redMaterial(TEXT("/Script/Engine.Material'/Game/Blueprint/Mat/M_RedCircle.M_RedCircle'"));
		static ConstructorHelpers::FObjectFinder<UMaterial> greenMaterial(TEXT("/Script/Engine.Material'/Game/Blueprint/Mat/M_GreenCircle.M_GreenCircle'"));
	
		if(redMaterial.Succeeded() && greenMaterial.Succeeded())
		{
			RedMat = redMaterial.Object;
			GreenMat = greenMaterial.Object;
		}
	//SmallCircleDecal->SetDecalMaterial(RedMat);
	//SmallCircleDecal->SetVisibility(true);
	//BigCircleDecal->SetDecalMaterial(GreenMat);
	//BigCircleDecal->SetVisibility(true);
	//
	//SmallCircleDecal->SetRelativeLocation(FVector(0.0f, 0.0f, 2.0f));
	//BigCircleDecal->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); 
	
}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	SmallCircle->SetVisibility(false);
	BigCircle->SetVisibility(false);

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
	UE_LOG(LogTemp, Warning, TEXT("Collision Channel: %d"), SmallCircleCol->GetCollisionObjectType());
	
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
		if(bCanAttack1)
		{
			Attack();
		}
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
		ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if(playerCharacter)
		{
			TargetPosition = playerCharacter->GetActorLocation();
		}
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
	
	// 머터리얼 빨간색
	SmallCircle->SetVisibility(true);
	FVector SmallCirclePosition = FVector(PlayerCharacter->GetActorLocation().X, PlayerCharacter->GetActorLocation().Y, PlayerCharacter->GetActorLocation().Z - 90.f);
	SmallCircle->SetWorldLocation(SmallCirclePosition);
	SmallCircle->SetMaterial(0, RedMat);

	// 머터리얼 초록색
	BigCircle->SetVisibility(true);
	FVector BigCirclePosition = FVector(PlayerCharacter->GetActorLocation().X, PlayerCharacter->GetActorLocation().Y, PlayerCharacter->GetActorLocation().Z - 93.f);
	BigCircle->SetWorldLocation(BigCirclePosition);
	BigCircle->SetMaterial(0, GreenMat);
	
	SplineComp->Init(GetActorLocation(), MidVector, PlayerCharacter->GetActorLocation());
	
	//GetWorld()->GetTimerManager().SetTimer(SplineTimer, this, &ABossEnemy::SplineMove, 3.f, false, 3.f);
	
}

void ABossEnemy::AttackPattern3()
{
	if (bIsAttack3Moving)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector Direction = (TargetPosition - CurrentLocation).GetSafeNormal();
		float Distance = FVector::Dist(CurrentLocation, TargetPosition);
		UE_LOG(LogTemp, Warning, TEXT("%f"), Distance);

		if (Distance <= ArrivalTarget)
		{
			// 목표 위치에 도달했음
			//SetActorLocation(TargetPosition);
			bIsAttack3Moving = false;
			GetWorld()->GetTimerManager().SetTimer(Attack3TimerHandle, this, &ThisClass::Attack3Arrived, 1.f,false);
		}
		else
		{
			// 이동을 계속합니다
			FVector NewLocation = CurrentLocation + (Direction * Attack3MoveSpeed * GetWorld()->GetDeltaSeconds());
			SetActorLocation(FVector(NewLocation.X, NewLocation.Y, GetActorLocation().Z));

			FRotator Rot = FRotator(GetActorRotation().Pitch, Direction.Rotation().Yaw, GetActorRotation().Roll);
			FQuat Dir = Rot.Quaternion();

			SetActorRotation(Dir);
		}
	}
}

void ABossEnemy::Attack3Arrived()
{
	
	for(int i=0; i < FirePoses.Num(); ++i)
	{
		float Angle = 15.f * i;

		FVector dir = FirePoses[i] - GetActorLocation();
		dir.Normalize();

		float Dis = FMath::RandRange(300.f, 1500.0f);

		FVector TargetLocation = FirePoses[i] + dir * Dis;
		FVector HighLocation = (TargetLocation + FirePoses[i])/2;
		HighLocation.Z = FMath::RandRange(200.f,400.f);

		FRotator Rot = dir.Rotation();
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(FirePoses[i]);
		SpawnTransform.SetRotation(Rot.Quaternion());

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		
		AInDirectEnemyBullet* bullet = GetWorld()->SpawnActor<AInDirectEnemyBullet>(InDirectBullet,SpawnTransform, SpawnParams);
		if(bullet)
		{
			bullet->InitialVar(TargetLocation, HighLocation, 1.f);
		}
	}
	bIsAttack3Moving = true;
	currentState = EEnemyState::Chasing;
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

void ABossEnemy::DelayBigCircleColOverlap()
{
	BigCircleCol->SetCollisionResponseToChannel(ECC_EngineTraceChannel5, ECR_Overlap);
	BigCircle->SetVisibility(false);
	SmallCircle->SetVisibility(false);
	currentState = EEnemyState::Chasing;
}

// 작은원오버랩
void ABossEnemy::OnSmallCircleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
		{
			ApplyDamageToPlayer(OtherActor);
			SmallCircleCol->SetCollisionResponseToChannel(ECC_EngineTraceChannel5, ECR_Ignore);
		}
	}
}

void ABossEnemy::OnBigCircleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if(APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
		{
			float Dis = FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());
			if(Dis >= SmallCircleCol->GetScaledSphereRadius())
			{
				ApplyDamageToPlayer(OtherActor);
			}
			SmallCircleCol->SetCollisionResponseToChannel(ECC_EngineTraceChannel5, ECR_Ignore);
		}
	}
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
			{
				// 2초동안 경로를 보여주는 함수
				if(bCanAttack1)
				{
					AttackPattern2();
					bCanAttack1 = false;
				}
				break;
			}
			
		case 2:
			{
				AttackPattern3();
			break;
				
			}
	}
}

void ABossEnemy::Die()
{
	
}


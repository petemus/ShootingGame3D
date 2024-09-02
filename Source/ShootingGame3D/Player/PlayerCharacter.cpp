 // Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../UI/GameOverWidget.h"
#include "ShootingGameModeBase.h"
#include "../Bullet/Bullet.h"
//#include "../Item/Item.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
	meshComp->SetupAttachment(RootComponent);
	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body StaticMesh"));
	bodyMesh->SetupAttachment(meshComp);
	headMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head StaticMesh"));
	headMesh->SetupAttachment(meshComp);
	
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	arrowComp->SetupAttachment(meshComp);
	leftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Arrow"));
	leftArrow->SetupAttachment(meshComp);
	rightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Arrow"));
	rightArrow->SetupAttachment(meshComp);

	// collision prest 세팅
	//UPrimitiveComponent* root = Cast<UPrimitiveComponent>(RootComponent);
	//if(root != nullptr) root->SetCollisionProfileName(TEXT("Player"));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// pc가져오고 subsystem 가져오고 subsys에서 imc와 연결
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	// 유효성 확인
	if (pc != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys != nullptr)
		{
			// 입력 subsystem에 imc 파일 변수 연결
			subsys->AddMappingContext(imc_playerInput, 0);
		}
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 총알 발사 시간을 tick에서 세주어야함
	nowTime += GetWorld()->GetDeltaSeconds();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// imc와 ia 연결
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);
	}
}



void APlayerCharacter::SetAttackMode(EItemType type)
{
	// item type에 따라 attack mode 다르게 설정
	switch (type)
	{
	case EItemType::BlueItem:
		myAttackMode = EAttackMode::GreatAttack;
		// 일정 시간 후에 다시 모드 변경
		// timer로 구현할까, tick으로 구현 할까 
		break;
	case EItemType::GreenItem:
		myAttackMode = EAttackMode::TripleAttack;
		break;
	case EItemType::PurpleItem:
		// 이벤트 호출 
		break;
	default:
		break;
	}

	// Timer에 대해서 공부 필요
	// 일정 시간 후로 다시 attack mode 변경 
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		// 내가 원하는 코드 구현
		myAttackMode = EAttackMode::NormalAttack;

		// 타이머 초기화
		GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
	}), attackTime, false);
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	// AddMovementInput(GetActorForwardVector(), MovementVector.Y); 사용 고려
	FVector2D moveVec = value.Get<FVector2D>();

	// 방향 벡터 구하기
	FVector dir = FVector(moveVec.X, moveVec.Y, 0);
	dir.Normalize();

	// 등속 이동 
	//FVector newLocation = GetActorLocation() + dir * moveSpeed * GetWorld()->GetDeltaSeconds();
	//SetActorLocation(newLocation);

	float Scalar = moveSpeed * GetWorld()->GetDeltaSeconds();
	// AddMovementInput의 dir은 worldDirection.. 
	// 상대 방향으로 바뀌어야하는데
	AddMovementInput(dir, Scalar);
	
	

	
}

void APlayerCharacter::Fire(const FInputActionValue& value)
{
	FVector2D vec = value.Get<FVector2D>();
	FVector dir = FVector(vec.X, vec.Y, 0);
	dir.Normalize();

	// character의 방향 변경
	// Find Look at Rotation -> start에서 Target으로 가리킬 회전캆 반환
	FRotator rotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + dir);
	meshComp->SetWorldRotation(rotate);
	
	// bullet forward 방향으로 spawn
	// spawn time 동안 spawn 못하게
	// Attack Mode에 따라 다른 공격
	switch (myAttackMode)
	{
	case EAttackMode::NormalAttack:
		if (nowTime >= spawnTime)
		{
			GetWorld()->SpawnActor<ABullet>(bulletFactory, arrowComp->GetComponentTransform());
			nowTime = 0;
		}
		break;
	case EAttackMode::GreatAttack:
		if (nowTime >= spawnTime)
		{
			GetWorld()->SpawnActor<ABullet>(bigbulletFactory, arrowComp->GetComponentTransform());
			nowTime = 0;
		}
		break;
	case EAttackMode::TripleAttack:
		if (nowTime >= spawnTime)
		{
			GetWorld()->SpawnActor<ABullet>(bulletFactory, arrowComp->GetComponentTransform());
			GetWorld()->SpawnActor<ABullet>(bulletFactory, leftArrow->GetComponentTransform());
			GetWorld()->SpawnActor<ABullet>(bulletFactory, rightArrow->GetComponentTransform());
			nowTime = 0;
		}
		break;
	default:
		break;
	}

}

void APlayerCharacter::SetDamaged(int32 Amount)
{

	Health -= Amount;

	if (Health <= 0)
	{
		Health = 0;

		// TODO GameOver
		AShootingGameModeBase* SGameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (SGameMode)
		{
			SGameMode->GameOverUI->SetVisibility(ESlateVisibility::Visible);

			APlayerController* PController = GetWorld()->GetFirstPlayerController();
			if (PController)
			{
				PController->bShowMouseCursor = true;
				PController->bEnableClickEvents = true;
			}
		}
	}
}


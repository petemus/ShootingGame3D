 // Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
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

	// Components ����
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

	// collision prest ����
	//UPrimitiveComponent* root = Cast<UPrimitiveComponent>(RootComponent);
	//if(root != nullptr) root->SetCollisionProfileName(TEXT("Player"));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// pc�������� subsystem �������� subsys���� imc�� ����
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	// ��ȿ�� Ȯ��
	if (pc != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys != nullptr)
		{
			// �Է� subsystem�� imc ���� ���� ����
			subsys->AddMappingContext(imc_playerInput, 0);
		}
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �Ѿ� �߻� �ð��� tick���� ���־����
	nowTime += GetWorld()->GetDeltaSeconds();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// imc�� ia ����
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);
	}
}



void APlayerCharacter::SetAttackMode(EItemType type)
{
	// item type�� ���� attack mode �ٸ��� ����
	switch (type)
	{
	case EItemType::BlueItem:
		myAttackMode = EAttackMode::GreatAttack;
		// ���� �ð� �Ŀ� �ٽ� ��� ����
		// timer�� �����ұ�, tick���� ���� �ұ� 
		break;
	case EItemType::GreenItem:
		myAttackMode = EAttackMode::TripleAttack;
		break;
	case EItemType::PurpleItem:
		// �̺�Ʈ ȣ�� 
		break;
	default:
		break;
	}

	// Timer�� ���ؼ� ���� �ʿ�
	// ���� �ð� �ķ� �ٽ� attack mode ���� 
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		// ���� ���ϴ� �ڵ� ����
		myAttackMode = EAttackMode::NormalAttack;

		// Ÿ�̸� �ʱ�ȭ
		GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
	}), attackTime, false);
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	// AddMovementInput(GetActorForwardVector(), MovementVector.Y); ��� ���
	FVector2D moveVec = value.Get<FVector2D>();

	// ���� ���� ���ϱ�
	FVector dir = FVector(moveVec.X, moveVec.Y, 0);
	dir.Normalize();

	// ��� �̵� 
	//FVector newLocation = GetActorLocation() + dir * moveSpeed * GetWorld()->GetDeltaSeconds();
	//SetActorLocation(newLocation);

	float Scalar = moveSpeed * GetWorld()->GetDeltaSeconds();
	// AddMovementInput�� dir�� worldDirection.. 
	// ��� �������� �ٲ����ϴµ�
	AddMovementInput(dir, Scalar);
	
	

	
}

void APlayerCharacter::Fire(const FInputActionValue& value)
{
	FVector2D vec = value.Get<FVector2D>();
	FVector dir = FVector(vec.X, vec.Y, 0);
	dir.Normalize();

	// character�� ���� ����
	// Find Look at Rotation -> start���� Target���� ����ų ȸ���� ��ȯ
	FRotator rotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + dir);
	meshComp->SetWorldRotation(rotate);
	
	// bullet forward �������� spawn
	// spawn time ���� spawn ���ϰ�
	// Attack Mode�� ���� �ٸ� ����
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


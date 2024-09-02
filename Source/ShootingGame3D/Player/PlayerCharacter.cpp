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

	// Components »ý¼º
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

	// collision prest ¼¼ÆÃ
	//UPrimitiveComponent* root = Cast<UPrimitiveComponent>(RootComponent);
	//if(root != nullptr) root->SetCollisionProfileName(TEXT("Player"));

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// pc°¡Á®¿À°í subsystem °¡Á®¿À°í subsys¿¡¼­ imc¿Í ¿¬°á
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	// À¯È¿¼º È®ÀÎ
	if (pc != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys != nullptr)
		{
			// ÀÔ·Â subsystem¿¡ imc ÆÄÀÏ º¯¼ö ¿¬°á
			subsys->AddMappingContext(imc_playerInput, 0);
		}
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ÃÑ¾Ë ¹ß»ç ½Ã°£À» tick¿¡¼­ ¼¼ÁÖ¾î¾ßÇÔ
	nowTime += GetWorld()->GetDeltaSeconds();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// imc¿Í ia ¿¬°á
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		enhancedInputComponent->BindAction(ia_fire, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);
	}
}



void APlayerCharacter::SetAttackMode(EItemType type)
{
	// item type¿¡ µû¶ó attack mode ´Ù¸£°Ô ¼³Á¤
	switch (type)
	{
	case EItemType::BlueItem:
		myAttackMode = EAttackMode::GreatAttack;
		// ÀÏÁ¤ ½Ã°£ ÈÄ¿¡ ´Ù½Ã ¸ðµå º¯°æ
		// timer·Î ±¸ÇöÇÒ±î, tickÀ¸·Î ±¸Çö ÇÒ±î 
		break;
	case EItemType::GreenItem:
		myAttackMode = EAttackMode::TripleAttack;
		break;
	case EItemType::PurpleItem:
		// ÀÌº¥Æ® È£Ãâ 
		break;
	default:
		break;
	}

	// Timer¿¡ ´ëÇØ¼­ °øºÎ ÇÊ¿ä
	// ÀÏÁ¤ ½Ã°£ ÈÄ·Î ´Ù½Ã attack mode º¯°æ 
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
	{
		// ³»°¡ ¿øÇÏ´Â ÄÚµå ±¸Çö
		myAttackMode = EAttackMode::NormalAttack;

		// Å¸ÀÌ¸Ó ÃÊ±âÈ­
		GetWorld()->GetTimerManager().ClearTimer(myTimerHandle);
	}), attackTime, false);
}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	// AddMovementInput(GetActorForwardVector(), MovementVector.Y); »ç¿ë °í·Á
	FVector2D moveVec = value.Get<FVector2D>();

	// ¹æÇâ º¤ÅÍ ±¸ÇÏ±â
	FVector dir = FVector(moveVec.X, moveVec.Y, 0);
	dir.Normalize();

	// µî¼Ó ÀÌµ¿ 
	//FVector newLocation = GetActorLocation() + dir * moveSpeed * GetWorld()->GetDeltaSeconds();
	//SetActorLocation(newLocation);

	float Scalar = moveSpeed * GetWorld()->GetDeltaSeconds();
	// AddMovementInputÀÇ dirÀº worldDirection.. 
	// »ó´ë ¹æÇâÀ¸·Î ¹Ù²î¾î¾ßÇÏ´Âµ¥
	AddMovementInput(dir, Scalar);
	
	

	
}

void APlayerCharacter::Fire(const FInputActionValue& value)
{
	FVector2D vec = value.Get<FVector2D>();
	FVector dir = FVector(vec.X, vec.Y, 0);
	dir.Normalize();

	// characterÀÇ ¹æÇâ º¯°æ
	// Find Look at Rotation -> start¿¡¼­ TargetÀ¸·Î °¡¸®Å³ È¸Àü¯“ ¹ÝÈ¯
	FRotator rotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + dir);
	meshComp->SetWorldRotation(rotate);
	
	// bullet forward ¹æÇâÀ¸·Î spawn
	// spawn time µ¿¾È spawn ¸øÇÏ°Ô
	// Attack Mode¿¡ µû¶ó ´Ù¸¥ °ø°Ý
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


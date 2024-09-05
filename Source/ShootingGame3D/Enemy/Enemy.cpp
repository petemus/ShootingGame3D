// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "../ShootingGameLogic/ShootingGameInstance.h"
#include "TimerManager.h"
#include "ShootingGame3D/Gimmick/RoomBase.h"
#include "ShootingGame3D/Player/PlayerCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBox"));
	CapsuleCol->SetupAttachment(RootComponent);

	CapsuleCol->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCapsuleOverlap);
	CapsuleCol->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnCapsuleEndOverlap);

	bIsPlayerOverlap = false;
	OverlapPlayer = nullptr;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


void AEnemy::Move(float DeltaTime)
{

}

void AEnemy::KnockBack()
{
}

void AEnemy::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		APawn* Player = Cast< APawn>(OtherActor);

		if (!Player) return;

		APlayerCharacter* PlayerPawn =  Cast<APlayerCharacter>(Player);
		if (PlayerPawn)
		{
			OverlapPlayer = PlayerPawn;
			bIsPlayerOverlap = true;

			ApplyDamageToPlayer(OtherActor);
			if (!GetWorldTimerManager().IsTimerActive(DamageHandle))
			{
				GetWorldTimerManager().SetTimer(DamageHandle, this, &AEnemy::ApplyContinueDamage, 2.0f, true, 2.0f);
			}
		}
	}

}

void AEnemy::OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* Player = Cast<APawn>(OtherActor);
	if (Player)
	{
		bIsPlayerOverlap = false;
		OverlapPlayer = nullptr;
		if (GetWorldTimerManager().IsTimerActive(DamageHandle))
		{
			GetWorldTimerManager().ClearTimer(DamageHandle);
		}
	}


}

void AEnemy::ApplyDamageToPlayer(AActor* Player)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player);

	if (!PlayerCharacter) return;

	IDamagedInterface* DamagedInter = Cast<IDamagedInterface>(PlayerCharacter);

	if (!DamagedInter) return;

	DamagedInter->SetDamaged(AttackStat);
}

void AEnemy::ApplyContinueDamage()
{
	if (OverlapPlayer && bIsPlayerOverlap)
	{
		ApplyDamageToPlayer(OverlapPlayer);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DamageHandle);
	}
}

void AEnemy::SetDamaged(int32 Amount)
{
	CurrentHealth -= Amount;
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		UGameInstance* GameIns = GetGameInstance();

		UShootingGameInstance* ShootingGameIns = Cast<UShootingGameInstance>(GameIns);

		if (ShootingGameIns)
		{
			ShootingGameIns->AddGold(EnemyGold);
		}

		// Room_Decrease
		IRoomInterface* OwnerRoomInterface = Cast<IRoomInterface>(GetOwner());
		if (OwnerRoomInterface)
		{
			OwnerRoomInterface->DecreaseCount();
		}
		
		Destroy();
	}
}


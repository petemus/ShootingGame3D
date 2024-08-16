// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

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

void AEnemy::Attack_Implementation()
{

}

void AEnemy::Move(float DeltaTime)
{

}

void AEnemy::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		ACharacter* Player = Cast< ACharacter>(OtherActor);
		if (Player)
		{
			OverlapPlayer = Player;
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
	ACharacter* Player = Cast<ACharacter>(OtherActor);
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
	UGameplayStatics::ApplyDamage(Player, AttackStat, nullptr, this, nullptr);
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


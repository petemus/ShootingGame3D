// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "../PlayerPawn.h"
#include "GameFramework/Pawn.h"
#include "../Public/DamagedInterface.h"
#include "Components/CapsuleComponent.h"
#include "ShootingGame3D/Player/PlayerCharacter.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCol"));
	SetRootComponent(CapsuleCol);

	CapsuleCol->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnCapsuleOverlap);

}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	TargetObj = Player;
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AEnemyBullet::Move(float Delta)
{
	
}

void AEnemyBullet::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor && (OtherActor != this) && (OtherComp->GetCollisionObjectType() == ECC_WorldDynamic || OtherComp->GetCollisionObjectType() == ECC_WorldStatic))
	{
		Destroy();
	}

	APawn* Player = Cast<APawn>(OtherActor);
	if (!Player) return;

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player);

	if (!PlayerCharacter) return;

	IDamagedInterface* DamagedInter = Cast<IDamagedInterface>(PlayerCharacter);

	if (DamagedInter)
	{
		DamagedInter->SetDamaged(Damage);
		Destroy();
	}
	
}



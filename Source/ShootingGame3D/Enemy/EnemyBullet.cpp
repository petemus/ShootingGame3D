// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"

#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleCol = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCol"));
	CapsuleCol->SetupAttachment(RootComponent);

	CapsuleCol->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnCapsuleOverlap);

}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBullet::Move(float Delta)
{
	FVector CurrentForward = GetActorForwardVector().GetSafeNormal();
	CurrentForward = CurrentForward * MoveSpeed * Delta;

	FVector NextLocation = CurrentForward + GetActorLocation();

	SetActorLocation(NextLocation);
}

void AEnemyBullet::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO

}



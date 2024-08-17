// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// capsule collider, static mesh 생성 
	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("My Capsule Component"));
	// collider root 컴포넌트로 설정
	SetRootComponent(capsuleComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My StaticMesh Component"));
	// capsuleComp에 부착
	meshComp->SetupAttachment(capsuleComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("My Arrow Component"));
	arrowComp->SetupAttachment(meshComp);

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	


	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


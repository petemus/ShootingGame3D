// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/FloatingPawnMovement.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// capsule collider, static mesh ���� 
	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("My Capsule Component"));
	// collider root ������Ʈ�� ����
	SetRootComponent(capsuleComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My StaticMesh Component"));
	// capsuleComp�� ����
	meshComp->SetupAttachment(capsuleComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("My Arrow Component"));
	arrowComp->SetupAttachment(meshComp);

	OurMovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("My Movement Component"));
	

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// �Է� subsystem imc ���Ͽ� mapping

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
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
// BeginPlay() ���� �� �ѹ� ���� 
// UInputComponent : unreal4 ���Ͽ� ���
// UEnhancedInputComponent : 5���� ��� 
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(ia_move, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
	}
}

void APlayerPawn::Move(const FInputActionValue& value)
{

	// AddMovementInput(GetActorForwardVector(), MovementVector.Y); ��� ���

	FVector2D moveVec = value.Get<FVector2D>();

	// ���� ���� ���ϱ�
	FVector dir = FVector(moveVec.X, moveVec.Y, 0);
	dir.Normalize();

	UE_LOG(LogTemp, Warning, TEXT(" % f : % f"), moveVec.X, moveVec.Y);
	//
	//// ��� �̵� 
	//FVector newLocation = GetActorLocation() + dir * moveSpeed * GetWorld()->GetDeltaSeconds();
	//SetActorLocation(newLocation);

	AddMovementInput(dir, moveSpeed * GetWorld()->GetDeltaSeconds());
}




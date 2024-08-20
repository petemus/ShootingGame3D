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

	// capsule collider, static mesh 생성 
	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("My Capsule Component"));
	// collider root 컴포넌트로 설정
	SetRootComponent(capsuleComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My StaticMesh Component"));
	// capsuleComp에 부착
	meshComp->SetupAttachment(capsuleComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("My Arrow Component"));
	arrowComp->SetupAttachment(meshComp);

	OurMovementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("My Movement Component"));
	

}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// 입력 subsystem imc 파일에 mapping

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
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
// BeginPlay() 실행 전 한번 실행 
// UInputComponent : unreal4 이하에 사용
// UEnhancedInputComponent : 5버전 사용 
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

	// AddMovementInput(GetActorForwardVector(), MovementVector.Y); 사용 고려

	FVector2D moveVec = value.Get<FVector2D>();

	// 방향 벡터 구하기
	FVector dir = FVector(moveVec.X, moveVec.Y, 0);
	dir.Normalize();

	UE_LOG(LogTemp, Warning, TEXT(" % f : % f"), moveVec.X, moveVec.Y);
	//
	//// 등속 이동 
	//FVector newLocation = GetActorLocation() + dir * moveSpeed * GetWorld()->GetDeltaSeconds();
	//SetActorLocation(newLocation);

	AddMovementInput(dir, moveSpeed * GetWorld()->GetDeltaSeconds());
}




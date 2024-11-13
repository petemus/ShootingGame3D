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
#include "../Enemy/Enemy.h"
#include "../Enemy/EnemyBullet.h"
#include "Kismet/GameplayStatics.h"
#include "../Public/ShootingGameModeBase.h"

 // enemy, enemy bullet

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components 생성	
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
	ArrowComp->SetupAttachment(GetMesh());
	LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Arrow"));
	LeftArrow->SetupAttachment(GetMesh());
	RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Arrow"));
	RightArrow->SetupAttachment(GetMesh());

	CircleArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Circle Arrow"));
	CircleArrow->SetupAttachment(RootComponent);


	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(RootComponent);*/
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if(Capsule == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("capsuele 없음"));
	}
	else Capsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnCapsuleOverlap);
	

	// pc를 가져와서 subsystem을 가져오고 subsys와 IMC를 연결
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (pc != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subsys
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys != nullptr)
		{
			subsys->AddMappingContext(PlayerInputIMC, 0);
		}
	}

	// 이벤트 바인딩
	OnAttackEvent.BindUFunction(this, FName("AttackCircle"));
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// nowTime을 Tick에서 세주어야함 
	NowTime += GetWorld()->GetDeltaSeconds();

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// imc와 IA 바인딩 
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		enhancedInputComponent->BindAction(MoveIA, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		enhancedInputComponent->BindAction(FireIA, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);
	}
}

void APlayerCharacter::KnockBack(AActor* OtherActor)
{
	// 실제로는 SetDamaged()에서 호출하면 other actor에 대한 정보를 얻을 수 없으므로
	// OnOverlapEvent에 하거나 아니면 SetDamage에서 actor에 대한 정보를 얻어야함
	// 근데 OnOverlapEvent에서 할려면 Enemybullet과 Enemy임을 확인해줘야 함

	// 힘 주기 -> AddImpulse() 사용 -> physics 적용해줘야 함
	// -> LaunchCharacter( velocity, true,ture);
	// XY축의 속도 무시, Z축 속도 무시 

	FVector knockBackDir = GetActorLocation() - OtherActor->GetActorLocation();
	knockBackDir.Normalize();
	knockBackDir.Z = 0;
	LaunchCharacter(knockBackDir * 1000, true, true);
	
}


 void APlayerCharacter::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
 {

	auto enemy = Cast<AEnemy>(OtherActor);
	auto enemyBullet = Cast<AEnemyBullet>(OtherActor);
	
	if(enemy != nullptr || enemyBullet != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCapsuleOverlap"));
		KnockBack(OtherActor);
	}
	
 }

void APlayerCharacter::SetAttackMode(EItemType type)
{
	// item type -> attack mode 다르게 할당
	switch (type)
	{
	case EItemType::BlueItem:
		myAttackMode = EAttackMode::GreatAttack;
		break;
	case EItemType::GreenItem:
		myAttackMode = EAttackMode::TripleAttack;
		break;
	case EItemType::PurpleItem:
		// 이벤트 발생
		if(OnAttackEvent.IsBound())
		{
			OnAttackEvent.Execute();
		}
		break;
	default:
		break;
	}

	// Timer
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			myAttackMode = EAttackMode::NormalAttack;
		}), AttackTime, false);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// Input에서 w, s를 x로 두고 a, d를 y로 두었음
	// 원래는 반대로 많이 하는 것 같음 

	
	// AddMovementInput(GetActorForwardVector(), MovementVector.Y); 
	FVector2D MoveVector = Value.Get<FVector2D>();

	// 방향 벡터 구하기
	// 이렇게 벡터를 구하면 플레이어의 방향과 상관없이 입력값의 방향만 받기 때문에 플레이어의 이동과는 상관이 없음
	// -> 이게 제대로 작동할려면 dir의 x, y와 캐릭터의 방향이 일치해야함
	FVector DirFwd = MoveVector.X * GetActorForwardVector();
	FVector DirBwd = MoveVector.Y * GetActorRightVector();

	FVector Dir = DirFwd + DirBwd;
	Dir.Normalize();

	float Scalar = MoveSpeed * GetWorld()->GetDeltaSeconds();
	// AddMovementInput의 dir은 worldDirection.. 
	AddMovementInput(Dir , Scalar);
	
	FRotator rotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + Dir);
	FRotator minus = FRotator(rotate.Pitch, rotate.Yaw  - 90.0f, 0);
	GetMesh()->SetWorldRotation(minus);


	
}

void APlayerCharacter::Fire(const FInputActionValue& Value)
{
	FVector2D vec = Value.Get<FVector2D>();
	FVector dir = FVector(vec.X, vec.Y, 0);
	dir.Normalize();
	

	// character 회전
	// mesh를 직접 회전하는 게 아니라 actor를 직접 회전하는 게 더 좋을듯
	// FindLookAtRotation은 짐벌락을 발생시킬 수 있으므로 다른 함수 사용하는 게 좋을듯 
	FRotator rotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), GetActorLocation() + dir );
	FRotator minus = FRotator(rotate.Pitch, rotate.Yaw  - 90.0f, 0);
	GetMesh()->SetWorldRotation(minus);
	
	// 왜 안돌릴까, arrow comp은 돌아가는 것 같은데
	/*SetActorRotation(rotate);*/
	
	
	switch (myAttackMode)
	{
	case EAttackMode::NormalAttack:
		if (NowTime >= SpawnTime)
		{
			GetWorld()->SpawnActor<ABullet>(BulletFactory, ArrowComp->GetComponentTransform());
			UGameplayStatics::PlaySound2D(GetWorld(), BulletSound);
			NowTime = 0;
		}
		break;
	case EAttackMode::GreatAttack:
		if (NowTime >= SpawnTime)
		{
			GetWorld()->SpawnActor<ABullet>(BigBulletFactory, ArrowComp->GetComponentTransform());
			UGameplayStatics::PlaySound2D(GetWorld(), BulletBigSound);
			NowTime = 0;
		}
		break;
	case EAttackMode::TripleAttack:
		if (NowTime >= SpawnTime)
		{
			GetWorld()->SpawnActor<ABullet>(BulletFactory, ArrowComp->GetComponentTransform());
			UGameplayStatics::PlaySound2D(GetWorld(), BulletSound);
			GetWorld()->SpawnActor<ABullet>(BulletFactory, LeftArrow->GetComponentTransform());
			UGameplayStatics::PlaySound2D(GetWorld(), BulletSound);
			GetWorld()->SpawnActor<ABullet>(BulletFactory, RightArrow->GetComponentTransform());
			UGameplayStatics::PlaySound2D(GetWorld(), BulletSound);
			NowTime = 0;
		}
		break;
	default:
		break;
	}

}

 void APlayerCharacter::AttackCircle()
 {
	// 현재 Circle Arrow에서 15도씩 0.2초마다 돌면서 bullet 발사
	// Arrow가 한바퀴 돌면 종료
	
	// Timer 호출
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
	{

		if(CircleArrowAngle >= 360.0f)
		{
			// 한바퀴 다 돌면 timer 종료
			CircleArrowAngle = 0;
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}
	
		GetWorld()->SpawnActor<ABullet>(BulletFactory, CircleArrow->GetComponentLocation(), CircleArrow->GetComponentRotation());
		//UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);
		
		// circle arrow가 actor의 위치 즉 상대 좌표에서의 0,0,0을 기준으로 이동하면서 회전해야함 
		FRotator newRotation = CircleArrow->GetComponentRotation();
		newRotation.Yaw += RotateAmout;
		CircleArrow->SetRelativeRotation(newRotation);
		/*FRotator newRotation = circlePivot->GetComponentRotation();
		newRotation.Yaw += rotateAmount;
		circlePivot->SetRelativeRotation(newRotation);*/
		// 이동 
		
		//FVector moveDir = circleArrow->GetForwardVector();
		//moveDir.Normalize();
		//FVector moveLoc = moveDir * 50;
		//circleArrow->SetWorldLocation(moveLoc + GetActorLocation());

		CircleArrowAngle += RotateAmout;
		
	}),  RotateTime, true);
	
 }

 void APlayerCharacter::RotateCircleArrow()
 {


	GetWorld()->SpawnActor<ABullet>(BulletFactory, CircleArrow->GetComponentLocation(), CircleArrow->GetComponentRotation());
	
	FRotator newRotation = CircleArrow->GetComponentRotation();
	newRotation.Yaw += RotateAmout;
	CircleArrow->SetWorldRotation(newRotation);
		
 }


 void APlayerCharacter::SetDamaged(int32 Amount)
{

	Health -= Amount;

	// 데미지 UI에 추가
	AShootingGameModeBase* GameModeBase = Cast<AShootingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if(GameModeBase != nullptr)
	{
		GameModeBase->HpSet(Health);
	}
	
	if (Health <= 0)
	{
		Health = 0;

		// TODO GameOver
		AShootingGameModeBase* SGameMode = Cast<AShootingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (SGameMode)
		{
			SGameMode->GameOver();
		}
	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Public/DamagedInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Item/Item.h"
#include "PlayerCharacter.generated.h"

UENUM()
enum class EAttackMode : uint8
{
	NormalAttack UMETA(DisplayName = "Normal Attak"),
	GreatAttack UMETA(DisplayName = "Great Attak"),
	TripleAttack  UMETA(DisplayName = "Triple Attak")
};


// 델리게이트 선언
DECLARE_DYNAMIC_DELEGATE(FOnAttackEvent);

class UStaticMeshComponent;
class UArrowComponent;

UCLASS()
class SHOOTINGGAME3D_API APlayerCharacter : public ACharacter, public IDamagedInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
public:
	// Components
	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* ArrowComp;
	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* LeftArrow;
	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* RightArrow;
	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* CircleArrow;


	// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* PlayerInputIMC;
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MoveIA;
	UPROPERTY(EditAnywhere, Category = "Input")
	 UInputAction* FireIA;

	// Bullet Factory
	// content browser에서의 값을 할당할려면 TSubclassOf
	// detail 창에서 할당할려면 그냥 클래스 
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	TSubclassOf<class ABullet> BulletFactory;
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	TSubclassOf<class ABullet> BigBulletFactory;

	// Sound
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	class USoundBase* BulletSound;
	UPROPERTY(EditDefaultsOnly, Category = "Bullet")
	class USoundBase* BulletBigSound;

public:
	// 설정 가능한 변수들 
	UPROPERTY(EditAnywhere, Category = "Setting")
	float MoveSpeed = 500;
	UPROPERTY(EditAnywhere, Category = "Setting")
	int32 Health = 6;
	UPROPERTY(EditAnywhere, Category = "Setting")
	float SpawnTime = 0.5f;
	// Default : 0.5
	UPROPERTY(EditAnywhere, Category = "Setting")
	int32 AttackStat = 1;

private:
	// spawn time 
	float NowTime = SpawnTime;
	// attack mode 
	EAttackMode MyAttackMode = EAttackMode::NormalAttack;
	float AttackTime = 5;
	// Circel Attack의 각도와 시간 변수
	float RotateAmout = 15;
	float RotateTime = 0.05f;
	// circle attack timerhandle
	FTimerHandle TimerHandle;
	
	// event 변수
	UPROPERTY()
	FOnAttackEvent OnAttackEvent;

	// 현재 circle arrow의 앵글을 저장할 변수
	float CircleArrowAngle = 0.0f;

// Function
public:
	// enum은 전방선언 불가
	void SetAttackMode(EItemType type);

private:
	// 입력 이벤츠 처리 함수
	void Move(const struct FInputActionValue& Value);
	// Fire 입력 이벤츠 처리 함수
	void Fire(const struct FInputActionValue& Value);
	// 공격 이벤트 발생시 실행할 함수
	// 이벤트에 바운드 될 함수는 UFUCTION 매크로 필요
	UFUNCTION()
	void AttackCircle();
	// 공격 이벤트에서 timer로 계속 반복하는 rotate 함수
	void RotateCircle();

	// void Knock-Back
	void KnockBack(AActor* OtherActor);
	
	// OnComponentbeginoverlap
	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						  int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetDamaged(int32 Amount) override;

};

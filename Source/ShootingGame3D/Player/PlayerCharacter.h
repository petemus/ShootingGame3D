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
	UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* bodyMesh;
	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* headMesh;
	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* arrowComp;
	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* leftArrow;
	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* rightArrow;
	UPROPERTY(EditAnywhere, Category = "Components")
	UArrowComponent* circleArrow;

	// IMC, IA
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_playerInput;

	UPROPERTY(EditAnywhere)
	class UInputAction* ia_move;

	UPROPERTY(EditAnywhere)
	 UInputAction* ia_fire;

	// Bullet Factory
	// content browser에서의 값을 할당할려면 TSubclassOf
	// detail 창에서 할당할려면 그냥 클래스 
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABullet> bulletFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABullet> bigbulletFactory;


public:
	
	UPROPERTY(EditAnywhere)
	float moveSpeed = 500;
	UPROPERTY(EditAnywhere)
	int32 Health = 6;
	UPROPERTY(EditAnywhere)
	float spawnTime = 0.5;
	UPROPERTY(EditAnywhere)
	int32 AttackStat = 1;

private:
	// spawn time 
	float nowTime = spawnTime;
	// attack mode 
	EAttackMode myAttackMode = EAttackMode::NormalAttack;
	float attackTime = 3;
	// Circel Attack의 각도와 시간 변수
	float rotateAmount = 15;
	float rotateTime = 0.1f;

	FTimerHandle timerHandle;

	
	// event 변수
	UPROPERTY()
	FOnAttackEvent OnAttackEvent;

private:
	// 현재 circle arrow의 앵글을 저장할 변수
	float circleArrowAngle = 0.0f;

public:
	// enum은 전방선언 불가
	void SetAttackMode(EItemType type);

private:
	// 입력 이벤츠 처리 함수
	void Move(const struct FInputActionValue& value);
	// Fire 입력 이벤츠 처리 함수
	void Fire(const struct FInputActionValue& value);
	// 공격 이벤트 발생시 실행할 함수
	// 이벤트에 바운드 될 함수는 UFUCTION 매크로 필요
	UFUNCTION()
	void AttackCircle();
	// 공격 이벤트에서 timer로 계속 반복하는 rotate 함수
	void RotateCircleArrow();

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetDamaged(int32 Amount) override;

};

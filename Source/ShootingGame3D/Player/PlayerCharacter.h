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
	// Character는 capsule, skeletal mesh, arrow comp를 기본으로 가지고 있음
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* bodyMesh;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* headMesh;
	UPROPERTY(EditAnywhere)
	UArrowComponent* arrowComp;
	UPROPERTY(EditAnywhere)
	UArrowComponent* leftArrow;
	UPROPERTY(EditAnywhere)
	UArrowComponent* rightArrow;

	// IMC, IA
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_playerInput;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_move;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_fire;

	// Bullet Factory
	// content browser에서 가져오므로 TSub
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABullet> bulletFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABullet> bigbulletFactory;

	// 일반 변수
	UPROPERTY(EditAnywhere)
	float moveSpeed = 500;
	UPROPERTY(EditAnywhere)
	int32 Health;
	UPROPERTY(EditAnywhere)
	float spawnTime = 0.5;
	UPROPERTY(EditAnywhere)
	int32 AttackStat = 1;

private:
	// spawn time 체크 해주는 변수
	float nowTime = spawnTime;
	// attack mode 
	EAttackMode myAttackMode = EAttackMode::NormalAttack;
	// attack mode 시간 체크
	float attackTime = 3;
	float leaveTime = 3;

public:
	// enum은 전방 선언 못하나????
	void SetAttackMode(EItemType type);

private:
	// 입력이 들어올때 호출되는 이벤트 함수
	void Move(const struct FInputActionValue& value);
	// Fire 이벤트 함수
	void Fire(const struct FInputActionValue& value);

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetDamaged(int32 Amount) override;

};

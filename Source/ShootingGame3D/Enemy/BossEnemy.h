// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyType.h"
#include "BossEnemy.generated.h"

class ADirectEnemyBullet;



UCLASS()
class SHOOTINGGAME3D_API ABossEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CalChasingTime();

	virtual void Move(float DeltaTime) override;

	virtual void SetDamaged(int32 Amount) override;

	UFUNCTION(BlueprintCallable)
	void AttackPattern1();

	UFUNCTION(BlueprintCallable)
	void AttackPattern2();
	
	// 발사 위치를 업데이트합니다.
	void UpdateFirePosition();

	void SplineMove();

public:
	/* StateFunc */
	void Idle();
	void Chase();
	void Attack();
	void Die();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> FirePoses;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ADirectEnemyBullet> Bullet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEnemyState currentState = EEnemyState::None;

public:
	/* Attack1 Value */
	FTimerHandle Attack1Timer;
	int32 Attack1Loop = 0;
	int32 Attack1Inx = 0;
	bool bCanAttack1 = true;

	/* Chase Value */
	float ChasingTime = 0.f;
	float ChasingMaxTime = 3.f;

	/* Attack2 Value (include Spline) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineActorComponent* SplineComp;
	FTimerHandle SplineTimer;


};


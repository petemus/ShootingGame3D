// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyType.h"
#include "BossEnemy.generated.h"

class AInDirectEnemyBullet;
class ADirectEnemyBullet;
class UMaterial;;

class UDecalComponent;

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
	void AttackPattern3();
	
	UFUNCTION(BlueprintCallable)
	void Attack3Arrived();
	
	// 발사 위치를 업데이트합니다.
	void UpdateFirePosition();
	void UpdateHealth();

	/* 어택2 함수 */
	UFUNCTION(BlueprintCallable)
	void AttackPattern2();
	void DelayBigCircleColOverlap();
	UFUNCTION(BlueprintCallable)
	void OnSmallCircleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void OnBigCircleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
	void SmallCircleNoCol();
	UFUNCTION(BlueprintCallable)
	void BigCircleNoCol();
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
	TSubclassOf<AInDirectEnemyBullet> InDirectBullet;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EEnemyState currentState = EEnemyState::None;

public:
	int32 RandomIdx = 0;
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
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	// UDecalComponent* SmallCircleDecal;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decal")
	//UDecalComponent* BigCircleDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
	UMaterial* RedMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
	UMaterial* GreenMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* SmallCircle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BigCircle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
	class USphereComponent* SmallCircleCol;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack2")
	USphereComponent* BigCircleCol;

	FTimerHandle SmallCircleHandle;
	FTimerHandle BigCircleHandle;

	/* Attack3 Value */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetPosition;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack3MoveSpeed = 700.f;
	
	bool bIsAttack3Moving = true;	
	float ArrivalTarget = 70.f;
	FTimerHandle Attack3TimerHandle;


	/* BossUI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UBossUI> BossUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBossUI* BossWidget;
};


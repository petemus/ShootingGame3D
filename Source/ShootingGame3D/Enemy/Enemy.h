// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Public/DamagedInterface.h"
#include "Enemy.generated.h"

class UCapsuleComponent;
class UPrimitiveComponent;

UCLASS(Abstract)
class SHOOTINGGAME3D_API AEnemy : public AActor, public IDamagedInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentHealth;

	UPROPERTY(BlueprintReadWrite)
	int32 MaxHealth;

	UPROPERTY(BlueprintReadWrite)
	float MoveSpeed;

	UPROPERTY(BlueprintReadWrite)
	int32 AttackStat;

	UPROPERTY(BlueprintReadWrite)
	int32 BulletAttackStat;

	UPROPERTY(BlueprintReadWrite)
	float AttackDuration;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleCol;

	UPROPERTY(BlueprintReadWrite)
	int32 EnemyGold;

protected:
	/* Damage */
	FTimerHandle DamageHandle;
	bool bIsPlayerOverlap;
	AActor* OverlapPlayer;


public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void Move(float DeltaTime);


	UFUNCTION(BlueprintCallable)
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnCapsuleEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ApplyDamageToPlayer(AActor* Player);

	UFUNCTION(BlueprintCallable)
	void ApplyContinueDamage();

public:
	/* DamagedInterface */
	UFUNCTION(BlueprintCallable)
	virtual void SetDamaged(int32 Amount) override;
};

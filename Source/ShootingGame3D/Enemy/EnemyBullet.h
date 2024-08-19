// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBullet.generated.h"


class UCapsuleComponent;
class UPrimitiveComponent;

UCLASS()
class SHOOTINGGAME3D_API AEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	/* Var */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MoveSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Damage;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleCol;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* TargetObj;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Move(float Delta);

	UFUNCTION(BlueprintCallable)
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SetDamage(int32 NewDamage) { Damage = NewDamage; }

};

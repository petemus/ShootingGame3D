// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class SHOOTINGGAME3D_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

public:
	// Components
	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	// 
	UPROPERTY(EditAnywhere)
	float speed = 600;
	UPROPERTY(EditAnywhere)
	int32 bulletDamage = 0;

public:
	void Move(float DeltaTime);

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
						 const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

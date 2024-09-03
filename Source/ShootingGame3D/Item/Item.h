// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

// Item의 종류를 나타내는 enum
UENUM()
enum class EItemType : uint8
{
	BlueItem UMETA(DisplayName = "Blue Item"),
	GreenItem UMETA(DisplayName = "Green Item"),
	PurpleItem UMETA(DisplayName = "Purple Item")
};

UCLASS()
class SHOOTINGGAME3D_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

public:
	// comp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;

	//
	UPROPERTY(EditAnywhere)
	EItemType myItemType;

public:
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

private:
	void Rotate(float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

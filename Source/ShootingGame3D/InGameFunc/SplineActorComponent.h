// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SplineActorComponent.generated.h"

class USplineMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SHOOTINGGAME3D_API USplineActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USplineActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void SetOwnerActor(AActor* InOwner);

	UFUNCTION(BlueprintCallable)
	void Init(FVector StartLocation, FVector MidLocation, FVector TargetLocation);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USplineMeshComponent*> SplineMeshComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* SplineMaterial;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* CreatedOwner;

	// 경로 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfPoints = 10;

	float DistanceAlong = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SplineTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SplineMaxTime = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 300.f;
	
	bool bIsSpline = false;

	// 실제 이동 관련 
	bool bCompletePath = false;
	float StartMoveTime = 0.f;
	float TotalMoveTime = 0.5f;
	FVector StartPosition;
	FVector MidPosition;
	FVector TargetPosition;

	// 경로들의 웅ㅁ직임
	TArray<float> SplineMeshDistances;
};

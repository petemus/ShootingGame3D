// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Public/DamagedInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

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
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* bodyMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* headMesh;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* arrowComp;
	UPROPERTY(EditAnywhere)


	// IMC, IA
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_playerInput;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_move;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_fire;

public:
	// 일반 변수
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float moveSpeed = 500;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpawnTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 AttackStat = 1;

private:
	// 입력이 들어올때 호출되는 이벤트 함수
	void Move(const struct FInputActionValue& value);

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetDamaged(int32 Amount) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Public/DamagedInterface.h"
#include "PlayerPawn.generated.h"

class UFloatingPawnMovement;

UCLASS()
class SHOOTINGGAME3D_API APlayerPawn : public APawn, public IDamagedInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

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
	// capsule collider
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* capsuleComp;
	// staticmesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* arrowComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFloatingPawnMovement* OurMovementComp;

	// IMC와 IA 바인딩을 위한 변수 선언
	// IMC
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_playerInput;
	// IA
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_move;

public:
	UPROPERTY(BlueprintReadWrite ,EditAnywhere)
	float moveSpeed = 500;

	UPROPERTY(BlueprintReadWrite ,EditAnywhere)
	int32 Health;

	UPROPERTY(BlueprintReadWrite ,EditAnywhere)
	float SpawnTime;

	UPROPERTY(BlueprintReadWrite ,EditAnywhere)
	int32 AttackStat = 1;

private:
	// 입력이 들어올때 호출되는 이벤트 함수
	void Move(const struct FInputActionValue& value);
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetDamaged(int32 Amount) override;

};

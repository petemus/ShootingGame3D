// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PlayerPawn.generated.h"

UCLASS()
class SHOOTINGGAME3D_API APlayerPawn : public APawn
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

	// capsule collider
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCapsuleComponent* capsuleComp;
	// staticmesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* arrowComp;

	// IMC�� IA ���ε��� ���� ���� ����
	// IMC
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_playerInput;
	// IA
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_move;

public:
	UPROPERTY(EditAnywhere)
	float moveSpeed = 500;

private:
	// �Է��� ���ö� ȣ��Ǵ� �̺�Ʈ �Լ�
	void Move(const struct FInputActionValue& value);


};

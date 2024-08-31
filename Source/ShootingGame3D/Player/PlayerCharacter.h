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
	// Character�� capsule, skeletal mesh, arrow comp�� �⺻���� ������ ����
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* bodyMesh;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* headMesh;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* arrowComp;

	// IMC, IA
	UPROPERTY(EditAnywhere)
	class UInputMappingContext* imc_playerInput;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_move;
	UPROPERTY(EditAnywhere)
	class UInputAction* ia_fire;

	// Bullet Factory
	// content browser���� �������Ƿ� TSub
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABullet> bulletFactory;

public:
	// �Ϲ� ����
	UPROPERTY(EditAnywhere)
	float moveSpeed = 500;
	UPROPERTY(EditAnywhere)
	int32 Health;
	UPROPERTY(EditAnywhere)
	float SpawnTime;
	UPROPERTY(EditAnywhere)
	int32 AttackStat = 1;

private:
	// �Է��� ���ö� ȣ��Ǵ� �̺�Ʈ �Լ�
	void Move(const struct FInputActionValue& value);
	// Fire �̺�Ʈ �Լ�
	void Fire(const struct FInputActionValue& value);

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetDamaged(int32 Amount) override;

};

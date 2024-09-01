// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Public/DamagedInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;

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
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere)
	 UStaticMeshComponent* bodyMesh;
	UPROPERTY(EditAnywhere)
	 UStaticMeshComponent* headMesh;
	UPROPERTY(EditAnywhere)
	class UArrowComponent* arrowComp;


	// IMC, IA
	UPROPERTY(EditAnywhere)
	UInputMappingContext* imc_playerInput;

	UPROPERTY(EditAnywhere)
	class UInputAction* ia_move;

	UPROPERTY(EditAnywhere)
	 UInputAction* ia_fire;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float moveSpeed = 500;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SpawnTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 AttackStat = 1;

private:
	void Move(const struct FInputActionValue& value);

public:
	UFUNCTION(BlueprintCallable)
	virtual void SetDamaged(int32 Amount) override;

};

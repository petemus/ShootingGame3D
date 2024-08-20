// Fill out your copyright notice in the Description page of Project Settings.


#include "LCUTESTCharacter.h"

// Sets default values
ALCUTESTCharacter::ALCUTESTCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALCUTESTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALCUTESTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALCUTESTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


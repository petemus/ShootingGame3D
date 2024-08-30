// Fill out your copyright notice in the Description page of Project Settings.


#include "BossController.h"


// Sets default values
ABossController::ABossController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABossController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


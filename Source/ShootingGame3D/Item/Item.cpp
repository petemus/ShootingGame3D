// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/StaticMeshComponent.h"	
#include "Components/SphereComponent.h"
#include "../Player/PlayerCharacter.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// comp 생성
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere collider"));
	SetRootComponent(sphereComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	meshComp->SetupAttachment(sphereComp);

	// collsiion setting
	sphereComp->SetCollisionProfileName(TEXT("Item"));
	// delegate 추가
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
							bool bFromSweep, const FHitResult& SweepResult)
{

	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);

	if (player != nullptr)
	{
		player->SetAttackMode(myItemType);
	}
	// item 파괴
	Destroy();
}

void AItem::Rotate(float DeltaTime)
{
	// 지속적으로 회전 
	FRotator addRotate = FRotator(0, 110 * DeltaTime, 0);
	AddActorLocalRotation(addRotate);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 회전 
	Rotate(DeltaTime);
}


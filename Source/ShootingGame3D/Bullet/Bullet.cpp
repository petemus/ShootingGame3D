// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Enemy/Enemy.h"
#include "../Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"		// Get Player character

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Components ����
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SetRootComponent(sphereComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh Component"));
	meshComp->SetupAttachment(sphereComp);

	// collision setting
	// �ϴ� preset���� ���
	sphereComp->SetCollisionProfileName(TEXT("PlayerBullet"));
	// �浹 �̺�Ʈ �Լ� ��������Ʈ�� ���
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnSphereOverlap);
}

void ABullet::Move(float DeltaTime)
{
	// ������ �̵� 
	FVector newLocation = GetActorLocation() + (DeltaTime * speed * GetActorForwardVector());
	SetActorLocation(newLocation);
}

void ABullet::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
							  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
							  bool bFromSweep, const FHitResult& SweepResult)
{
	// overlap -> worldstatic : Destroy
	// overlap -> Enemy : Damage enemy
	if (OtherComp->GetCollisionObjectType() == ECC_WorldStatic)
	{
		Destroy();
	}
	else
	{
		// Enemy cast
		AEnemy* enemy = Cast<AEnemy>(OtherActor);
		if (enemy != nullptr)
		{
			// Player�� ��� ���� �ñ�...
			APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (player != nullptr)
			{
				// ������ �ٷ� �����ϴ� �� ���� ��������...
				enemy->SetDamaged(player->AttackStat);
				Destroy();
			}
		}
	}
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}


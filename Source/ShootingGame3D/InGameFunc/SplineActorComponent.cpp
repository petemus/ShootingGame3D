// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineActorComponent.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "ShootingGame3D/Enemy/BossEnemy.h"


// Sets default values for this component's properties
USplineActorComponent::USplineActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	NumberOfPoints = 10;
}


// Called when the game starts
void USplineActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	for(int32 i =0; i<NumberOfPoints; ++i)
	{
		USplineMeshComponent* SplineMeshComp = NewObject<USplineMeshComponent>(this);
		SplineMeshComp->RegisterComponent();
		SplineMeshComp->SetMobility(EComponentMobility::Type::Movable);
		if(SplineMesh)
		{
			SplineMeshComp->SetStaticMesh(SplineMesh);
		}
        
		if (SplineMaterial)
		{
			SplineMeshComp->SetMaterial(0, SplineMaterial);
		}
		
		SplineMeshComponents.Add(SplineMeshComp);
	}
}


// Called every frame
void USplineActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if(bIsSpline)
	{
		SplineTime += DeltaTime;
		
		for(int32 i =0; i< NumberOfPoints ; ++i)
		{
						
			SplineMeshDistances[i] += + DeltaTime*MoveSpeed;

			if (SplineMeshDistances[i] > SplineComponent->GetSplineLength())
			{
				SplineMeshDistances[i] -= SplineComponent->GetSplineLength();
			}
			
			float Distance = SplineMeshDistances[i];
			
			FVector NewLocation =  SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
			FVector Tangent = SplineComponent->GetTangentAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

			float NextDis = Distance + SplineComponent->GetSplineLength()/(NumberOfPoints*3);

			if (NextDis > SplineComponent->GetSplineLength())
			{
				NextDis -= SplineComponent->GetSplineLength();
			}
			
			FVector NewLocation2 =  SplineComponent->GetLocationAtDistanceAlongSpline(NextDis, ESplineCoordinateSpace::World);
			FVector Tangent2 = SplineComponent->GetTangentAtDistanceAlongSpline(NextDis, ESplineCoordinateSpace::World);


			SplineMeshComponents[i]->SetStartAndEnd(NewLocation, Tangent, NewLocation2, Tangent2);
			
		}

		if(SplineTime >= SplineMaxTime)
		{
			SplineTime = 0.f;
			bIsSpline = false;

			for(auto SpMesh : SplineMeshComponents)
			{
				SpMesh->SetVisibility(false);
			}
			bCompletePath = true;
			StartMoveTime =GetWorld()->GetTimeSeconds();
		}
	}

	if(bCompletePath)
	{
		float MoveRatio = (GetWorld()->GetTimeSeconds() - StartMoveTime)/ TotalMoveTime;
				
		if(MoveRatio >= 1.f)
		{
			MoveRatio = 1.f;
		}
		
		FVector L1 = FMath::Lerp(StartPosition, MidPosition, MoveRatio);
		FVector L2 = FMath::Lerp(MidPosition, TargetPosition, MoveRatio);

		FVector NewLocation = FMath::Lerp(L1, L2, MoveRatio);
		
		CreatedOwner->SetActorLocation(NewLocation);

		if(MoveRatio >= 1.f)
		{
			bCompletePath = false;
			MoveRatio = 0.f;
		}
	}
}

void USplineActorComponent::SetOwnerActor(AActor* InOwner)
{
	CreatedOwner = InOwner;
}

void USplineActorComponent::Init(FVector StartLocation, FVector MidLocation, FVector TargetLocation)
{
	StartPosition = StartLocation;
	MidPosition = MidLocation;
	TargetPosition = TargetLocation;
	
	SplineComponent->ClearSplinePoints();

	SplineComponent->AddSplinePoint(StartLocation, ESplineCoordinateSpace::World,false);
	SplineComponent->AddSplinePoint(MidLocation, ESplineCoordinateSpace::World,false);
	SplineComponent->AddSplinePoint(TargetLocation, ESplineCoordinateSpace::World,false);

	SplineComponent->UpdateSpline();

	for(int32 i =0; i< NumberOfPoints; ++i)
	{
		bIsSpline = true;
				
		float Distance = SplineComponent->GetSplineLength() * i / NumberOfPoints;

		FVector NewLocation =  SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FVector Tangent = SplineComponent->GetTangentAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		float NextDis = Distance + SplineComponent->GetSplineLength()/NumberOfPoints;
		
		FVector NewLocation2 =  SplineComponent->GetLocationAtDistanceAlongSpline(NextDis, ESplineCoordinateSpace::World);
		FVector Tangent2 = SplineComponent->GetTangentAtDistanceAlongSpline(NextDis, ESplineCoordinateSpace::World);


		SplineMeshComponents[i]->SetStartAndEnd(NewLocation, Tangent, NewLocation2, Tangent2);
		
		FVector SplinePoint = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		
	}
	SplineMeshDistances.Empty();
	SplineMeshDistances.SetNum(NumberOfPoints);

	for (int32 i = 0; i < NumberOfPoints; ++i)
	{
		SplineMeshDistances[i] = SplineComponent->GetSplineLength() * i / NumberOfPoints;
		SplineMeshComponents[i]->SetVisibility(true);
	}
	
}


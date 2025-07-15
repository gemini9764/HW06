#include "MoveScaffolding.h"

AMoveScaffolding::AMoveScaffolding()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;

	MoveDistance = FVector(0.0f, 0.0f, 200.0f);
	MoveSpeed = 50.0f;
	bMovingForward = true;
}

void AMoveScaffolding::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	TargetLocation = StartLocation + MoveDistance;
}

void AMoveScaffolding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	FVector Direction = (bMovingForward) ? (TargetLocation - CurrentLocation) : (StartLocation - CurrentLocation);

	Direction.Normalize();
	FVector NewLocation = CurrentLocation + (Direction * MoveSpeed * DeltaTime);

	SetActorLocation(NewLocation);
	
	if (bMovingForward)
	{
		if (FVector::DistSquared(NewLocation, TargetLocation) < 1.0f)
		{
			bMovingForward = false;
		}
	}
	else
	{
		if (FVector::DistSquared(NewLocation, StartLocation) < 1.0f)
		{
			bMovingForward = true;
		}
	}
}
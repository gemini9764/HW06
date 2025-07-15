#include "RotateScaffolding.h"

ARotateScaffolding::ARotateScaffolding()
{
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	RootComponent = PlatformMesh;

	RotationSpeed = 45.0f;
	RotationAxis = FRotator(0.0f, 1.0f, 0.0f);
}

void ARotateScaffolding::BeginPlay()
{
	Super::BeginPlay();
}

void ARotateScaffolding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
	FRotator DeltaRotation = RotationAxis * RotationSpeed * DeltaTime;

	SetActorRotation(CurrentRotation + DeltaRotation);
}
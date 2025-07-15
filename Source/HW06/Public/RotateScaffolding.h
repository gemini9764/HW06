#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotateScaffolding.generated.h"

UCLASS()
class HW06_API ARotateScaffolding : public AActor
{
	GENERATED_BODY()

public:
	ARotateScaffolding();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	float RotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	FRotator RotationAxis;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	float DesiredSpawnClearance;
};
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveScaffolding.generated.h"

UCLASS()
class HW06_API AMoveScaffolding : public AActor
{
	GENERATED_BODY()

public:
	AMoveScaffolding();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector MoveDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	float DesiredSpawnClearance;

private:
	FVector StartLocation;
	FVector TargetLocation;
	bool bMovingForward;
};

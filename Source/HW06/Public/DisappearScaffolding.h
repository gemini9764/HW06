#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DisappearScaffolding.generated.h"

UCLASS()
class HW06_API ADisappearScaffolding : public AActor
{
	GENERATED_BODY()

public:
	ADisappearScaffolding();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector TargetMoveLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing")
	float ReappearDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn")
	float DesiredSpawnClearance;

private:
	FVector StartLocation;
	bool bMovingToTarget;
	bool bHasReachedTarget;

	FTimerHandle TimerHandle_Reappear; 

	void Disappear();
	void Reappear();
};

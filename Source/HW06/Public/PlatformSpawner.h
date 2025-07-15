#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveScaffolding.h"
#include "RotateScaffolding.h"
#include "DisappearScaffolding.h"
#include "PlatformSpawner.generated.h"

UCLASS()
class HW06_API APlatformSpawner : public AActor
{
	GENERATED_BODY()

public:
	APlatformSpawner();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	TArray<TSubclassOf<AActor>> PlatformTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings", meta = (ClampMin = "1"))
	int32 NumberOfPlatformsToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	FVector SpawnCenterLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings", meta = (ClampMin = "0.0"))
	float SpawnRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings", meta = (ClampMin = "0.01"))
	float SpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Settings")
	bool bSpawnOnBeginPlay;

	UFUNCTION(BlueprintCallable, Category = "Spawn Functions")
	void SpawnRandomPlatform();

	UFUNCTION(BlueprintCallable, Category = "Spawn Functions")
	void SpawnSpecificPlatform(TSubclassOf<AActor> PlatformClass, FVector Location);

private:
	FTimerHandle TimerHandle_SpawnPlatforms;
	int32 CurrentSpawnCount;
	
	UPROPERTY(EditDefaultsOnly, Category = "Spawn Settings")
	int32 MaxSpawnAttempts;

	void SpawnPlatformsLoop();
	
	float GetPlatformClearance(TSubclassOf<AActor> PlatformClass) const;
};

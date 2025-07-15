#include "PlatformSpawner.h"

#include "Kismet/KismetSystemLibrary.h"

APlatformSpawner::APlatformSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    NumberOfPlatformsToSpawn = 5;
    SpawnCenterLocation = FVector(0.0f, 0.0f, 0.0f);
    SpawnRadius = 500.0f;
    SpawnInterval = 0.5f;
    bSpawnOnBeginPlay = true;
    CurrentSpawnCount = 0;
    MaxSpawnAttempts = 10;
}

void APlatformSpawner::BeginPlay()
{
    Super::BeginPlay();

    if (bSpawnOnBeginPlay)
    {
        CurrentSpawnCount = 0;
        GetWorldTimerManager().SetTimer(TimerHandle_SpawnPlatforms, this, &APlatformSpawner::SpawnPlatformsLoop, SpawnInterval, true);
    }
}

void APlatformSpawner::SpawnPlatformsLoop()
{
    if (CurrentSpawnCount < NumberOfPlatformsToSpawn)
    {
        SpawnRandomPlatform();
        CurrentSpawnCount++;
    }
    else
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_SpawnPlatforms);
        UE_LOG(LogTemp, Log, TEXT("All %d platforms spawned."), NumberOfPlatformsToSpawn);
    }
}

float APlatformSpawner::GetPlatformClearance(TSubclassOf<AActor> PlatformClass) const
{
    if (!PlatformClass) return 0.0f;
    
    if (AMoveScaffolding* DefaultMovingPlatform = Cast<AMoveScaffolding>(PlatformClass->GetDefaultObject()))
    {
        return DefaultMovingPlatform->DesiredSpawnClearance;
    }
    else if (ARotateScaffolding* DefaultRotatingPlatform = Cast<ARotateScaffolding>(PlatformClass->GetDefaultObject()))
    {
        return DefaultRotatingPlatform->DesiredSpawnClearance;
    }
    else if (ADisappearScaffolding* DefaultDisappearingPlatform = Cast<ADisappearScaffolding>(PlatformClass->GetDefaultObject()))
    {
        return DefaultDisappearingPlatform->DesiredSpawnClearance;
    }

    return 0.0f;
}


void APlatformSpawner::SpawnRandomPlatform()
{
    if (PlatformTypes.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlatformTypes array is empty in PlatformSpawner! Cannot spawn."));
        return;
    }

    TSubclassOf<AActor> PlatformToSpawnClass = PlatformTypes[FMath::RandRange(0, PlatformTypes.Num() - 1)];
    if (!PlatformToSpawnClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected platform class is null. Skipping spawn."));
        return;
    }

    FVector FinalSpawnLocation = FVector::ZeroVector;
    bool bLocationFound = false;

    for (int32 Attempt = 0; Attempt < MaxSpawnAttempts; ++Attempt)
    {
        FVector RandomOffset = FMath::RandPointInBox(FBox(FVector(-SpawnRadius, -SpawnRadius, -SpawnRadius), FVector(SpawnRadius, SpawnRadius, SpawnRadius)));
        FVector ProposedSpawnLocation = SpawnCenterLocation + RandomOffset;

        float ProposedPlatformClearance = GetPlatformClearance(PlatformToSpawnClass);
        if (ProposedPlatformClearance <= 0.0f)
        {
            ProposedPlatformClearance = 150.0f;
            UE_LOG(LogTemp, Warning, TEXT("Could not get DesiredSpawnClearance for %s. Using default 150.0f."), *PlatformToSpawnClass->GetName());
        }
        
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(this);

        TArray<AActor*> OverlappingActors;
        
        bool bHasOverlap = UKismetSystemLibrary::SphereOverlapActors(
            GetWorld(),
            ProposedSpawnLocation,
            ProposedPlatformClearance,
            ObjectTypes,
            AActor::StaticClass(),
            ActorsToIgnore,
            OverlappingActors
        );

        if (!bHasOverlap)
        {
            FinalSpawnLocation = ProposedSpawnLocation;
            bLocationFound = true;
            break;
        }
    }

    if (!bLocationFound)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find a non-overlapping spawn location after %d attempts for platform type %s!"), MaxSpawnAttempts, *PlatformToSpawnClass->GetName());
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AActor* SpawnedPlatform = GetWorld()->SpawnActor<AActor>(PlatformToSpawnClass, FinalSpawnLocation, FRotator::ZeroRotator, SpawnParams);

    if (SpawnedPlatform)
    {
        UE_LOG(LogTemp, Log, TEXT("Spawned %s at %s"), *SpawnedPlatform->GetName(), *SpawnedPlatform->GetActorLocation().ToString());
    }
}

void APlatformSpawner::SpawnSpecificPlatform(TSubclassOf<AActor> PlatformClass, FVector Location)
{
    if (!PlatformClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlatformClass is null in SpawnSpecificPlatform! Cannot spawn."));
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AActor* SpawnedPlatform = GetWorld()->SpawnActor<AActor>(PlatformClass, Location, FRotator::ZeroRotator, SpawnParams);

    if (SpawnedPlatform)
    {
        UE_LOG(LogTemp, Log, TEXT("Spawned specific %s at %s"), *SpawnedPlatform->GetName(), *SpawnedPlatform->GetActorLocation().ToString());
    }
}
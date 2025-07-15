#include "DisappearScaffolding.h"

ADisappearScaffolding::ADisappearScaffolding()
{
    PrimaryActorTick.bCanEverTick = true;

    PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
    RootComponent = PlatformMesh;
    
    TargetMoveLocation = FVector(0.0f, 0.0f, 500.0f);
    MoveSpeed = 100.0f;
    
    ReappearDelay = 3.0f;

    bMovingToTarget = true;
    bHasReachedTarget = false;
}

void ADisappearScaffolding::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();
}

void ADisappearScaffolding::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (bMovingToTarget && !bHasReachedTarget)
    {
        FVector CurrentLocation = GetActorLocation();
        FVector Direction = (TargetMoveLocation - CurrentLocation).GetSafeNormal();

        FVector NewLocation = CurrentLocation + (Direction * MoveSpeed * DeltaTime);
        SetActorLocation(NewLocation);
        
        if (FVector::DistSquared(NewLocation, TargetMoveLocation) < FMath::Square(MoveSpeed * DeltaTime * 2.0f) ||
            FVector::DotProduct(Direction, (TargetMoveLocation - NewLocation).GetSafeNormal()) < 0.0f)
        {
            SetActorLocation(TargetMoveLocation);
            bHasReachedTarget = true;
            bMovingToTarget = false;

            Disappear();
        }
    }
}

void ADisappearScaffolding::Disappear()
{
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    
    GetWorldTimerManager().SetTimer(TimerHandle_Reappear, this, &ADisappearScaffolding::Reappear, ReappearDelay, false);
}

void ADisappearScaffolding::Reappear()
{
    SetActorHiddenInGame(false);
    SetActorEnableCollision(true);

    SetActorLocation(StartLocation);
    bHasReachedTarget = false;
    bMovingToTarget = true;
}
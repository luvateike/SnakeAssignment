#include "SCR_Gamemode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

ASCR_Gamemode::ASCR_Gamemode()
{
	PlayerControllerClass = ASCR_PlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<ASCR_Apple> AppleBP(TEXT("/Game/Blueprints/BP_Apple"));
	if (AppleBP.Succeeded())
	{
		AppleBlueprint = AppleBP.Class;
	}
}

void ASCR_Gamemode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (AppleBlueprint)
	{
		World->GetTimerManager().SetTimer(AppleSpawnTimerHandle, this, &ASCR_Gamemode::SpawnApple, AppleSpawnInterval, true);
	}

	SpawnWalls();
}

void ASCR_Gamemode::SpawnApple()
{
	if (!AppleBlueprint) return;

	UWorld* World = GetWorld();
	if (!World) return;

	float RandX = FMath::RandRange(-BoundX, BoundX);
	float RandY = FMath::RandRange(-BoundY, BoundY);
	FVector SpawnLocation(RandX, RandY, 100.f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	World->SpawnActor<ASCR_Apple>(AppleBlueprint, SpawnLocation, SpawnRotation, Params);
}

void ASCR_Gamemode::SpawnWalls()
{
	if (!WallBlueprint) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector WallSize = FVector(100.f, BoundY * 2.f + 200.f, 500.f);

	TArray<FVector> Locations = {
		FVector(-BoundX - 200.f, 0.f, 0),
		FVector(BoundX + 200.f, 0.f, 0.f),
		FVector(0.f, -BoundY - 200.f, 0),
		FVector(0.f, BoundY + 200.f, 0) 
	};

	TArray<FRotator> Rotations = {
		FRotator::ZeroRotator,
		FRotator::ZeroRotator,
		FRotator(0.f, 90.f, 0.f),
		FRotator(0.f, 90.f, 0.f)
	};

	for (int32 i = 0; i < 4; i++)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		World->SpawnActor<AActor>(WallBlueprint, Locations[i], Rotations[i], Params);
	}
}

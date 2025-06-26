#include "SCR_Gamemode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

ASCR_Gamemode::ASCR_Gamemode()
{
	PlayerControllerClass = ASCR_PlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> SnakeBP(TEXT("/Game/Blueprints/BP_Player"));
	if (SnakeBP.Succeeded())
	{
		AISnakeClass = SnakeBP.Class;
	}

	AIControllerClass = ASCR_PlayerAIController::StaticClass();

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
	if (!World || !AISnakeClass || !AIControllerClass) return;

	FVector SnakeSpawnLocation(100.f, 100.f, 100.f);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters PawnParams;
	PawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ASCR_Player* AISnake = World->SpawnActor<ASCR_Player>(AISnakeClass, SnakeSpawnLocation, SpawnRotation, PawnParams);
	if (!AISnake) return;

	AController* AIController = World->SpawnActor<AController>(AIControllerClass);
	if (AIController)
	{
		AIController->Possess(AISnake);
	}

	if (AppleBlueprint)
	{
		World->GetTimerManager().SetTimer(AppleSpawnTimerHandle, this, &ASCR_Gamemode::SpawnApple, AppleSpawnInterval, true);
	}
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

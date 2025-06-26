#include "SCR_Gamemode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "SnakeAssignment/UserInterface/SCR_OutroScoreDisplay.h"
#include "SnakeAssignment/UserInterface/SCR_ScoresDisplay.h"
#include "UObject/ConstructorHelpers.h"

ASCR_Gamemode::ASCR_Gamemode()
{
	PlayerControllerClass = ASCR_PlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<ASCR_Apple> AppleBP(TEXT("/Game/Blueprints/BP_Apple"));
	if (AppleBP.Succeeded())
	{
		AppleBlueprint = AppleBP.Class;
	}

	LevelAppleSpawnIntervals = { 1.0f, .5f, .2f };
	LevelBoundsX = { 3000.f, 1500.f, 750.f };
	LevelBoundsY = { 3000.f, 1500.f, 750.f };
}

void ASCR_Gamemode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (AppleBlueprint)
	{
		World->GetTimerManager().SetTimer(AppleSpawnTimerHandle, this, &ASCR_Gamemode::SpawnApple, AppleSpawnInterval, true);
	}

	World->GetTimerManager().SetTimer(UserInterfaceTimerHandle, this, &ASCR_Gamemode::UpdateUI, UICheckInterval, true);

	SpawnWalls();
	
	SetGameState(EGameState::MainMenu);
	if (MainMenuDisplay)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuDisplay);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}
}

void ASCR_Gamemode::SetGameState(EGameState NewState)
{
	if (CurrentGameState == NewState) return;

	CurrentGameState = NewState;

	switch (CurrentGameState)
	{
	case EGameState::MainMenu:
		UE_LOG(LogTemp, Warning, TEXT("State changed to: MainMenu"));
		
		if (OutroWidget) OutroWidget->RemoveFromViewport();
		if (MainMenuDisplay)
		{
			MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuDisplay);
			if (MainMenuWidget)
			{
				MainMenuWidget->AddToViewport();
			}
		}
		break;

	case EGameState::Game:
		UE_LOG(LogTemp, Warning, TEXT("State changed to: Game"));
		if (MainMenuWidget) MainMenuWidget->RemoveFromViewport();
		if (ScoresDisplayClass)
		{
			ScoresDisplayWidget = Cast<USCR_ScoresDisplay>(CreateWidget<UUserWidget>(GetWorld(), ScoresDisplayClass));
			if (ScoresDisplayWidget)
			{
				ScoresDisplayWidget->AddToViewport();
			}
		}
		break;

	case EGameState::Outro:
		UE_LOG(LogTemp, Warning, TEXT("State changed to: Outro"));
		if (ScoresDisplayWidget) ScoresDisplayWidget->RemoveFromViewport();
		if (OutroDisplay)
		{
			OutroWidget = Cast<USCR_OutroScoreDisplay>(CreateWidget<UUserWidget>(GetWorld(), OutroDisplay));
			if (OutroWidget)
			{
				OutroWidget->AddToViewport();
				OutroWidget->SetText(TotalScoresText);
			}
		}
		
		GetWorldTimerManager().SetTimer(QuitGameTimerHandle, this, &ASCR_Gamemode::QuitGameAfterOutro, 5.0f, false);
		break;
	}
}

void ASCR_Gamemode::QuitGameAfterOutro()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void ASCR_Gamemode::UpdateUI()
{
	if (CurrentGameState != EGameState::Game) return;

	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCR_Player::StaticClass(), PlayerActors);

	FString ScoreText = "Scores:\n";
	bool AllPlayersDead = true;

	for (AActor* Actor : PlayerActors)
	{
		ASCR_Player* Player = Cast<ASCR_Player>(Actor);
		if (Player)
		{
			FString PlayerLabel = Player->bIsAI ? "AI" : "Player";
			ScoreText += FString::Printf(TEXT("%s %d: %d\n"), *PlayerLabel, Player->playerNumber, Player->Body.Num());

			if (!RegisteredPlayerScores.Contains(Player))
			{
				Player->OnDeath.AddDynamic(this, &ASCR_Gamemode::GetLastScore);
				RegisteredPlayerScores.Add(Player);
			}

			if (IsValid(Player) && !Player->IsActorBeingDestroyed())
			{
				AllPlayersDead = false;
			}
		}
	}

	if (ScoresDisplayWidget)
	{
		ScoresDisplayWidget->SetText(ScoreText);
	}

	if (AllPlayersDead)
	{
		AdvanceLevel();
	}
}

void ASCR_Gamemode::GetLastScore(AActor* DeadPlayer)
{
	ASCR_Player* Player = Cast<ASCR_Player>(DeadPlayer);
	if (Player)
	{
		FString PlayerLabel = Player->bIsAI ? "AI" : "Player";
		TotalScoresText += FString::Printf(TEXT("%s %d: %d\n"), *PlayerLabel, Player->playerNumber, Player->Body.Num());
	}
}

void ASCR_Gamemode::SpawnApple()
{
	if (CurrentGameState != EGameState::Game) return;

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

		AActor* wall = World->SpawnActor<AActor>(WallBlueprint, Locations[i], Rotations[i], Params);
		Walls.Add(wall);
	}
}

void ASCR_Gamemode::AdvanceLevel()
{
	CurrentLevel++;

	TArray<AActor*> TailActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCR_Tail::StaticClass(), TailActors);

	for (AActor* Tail : TailActors)
	{
		if (IsValid(Tail))
		{
			Tail->Destroy();
		}
	}

	if (CurrentLevel >= MaxLevels)
	{
		SetGameState(EGameState::Outro);
		return;
	}

	BoundX = LevelBoundsX[CurrentLevel];
	BoundY = LevelBoundsY[CurrentLevel];

	if (Walls.Num() == 4)
	{
		Walls[0]->SetActorLocation(FVector(-BoundX - 200.f, 0.f, 0.f));
		Walls[1]->SetActorLocation(FVector(BoundX + 200.f, 0.f, 0.f));
		Walls[2]->SetActorLocation(FVector(0.f, -BoundY - 200.f, 0.f));
		Walls[3]->SetActorLocation(FVector(0.f, BoundY + 200.f, 0.f));
	}

	AppleSpawnInterval = LevelAppleSpawnIntervals[CurrentLevel];

	TArray<AActor*> ExistingApples;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCR_Apple::StaticClass(), ExistingApples);
	for (AActor* Apple : ExistingApples)
	{
		if (IsValid(Apple))
		{
			Apple->Destroy();
		}
	}

	GetWorldTimerManager().ClearTimer(AppleSpawnTimerHandle);
	GetWorldTimerManager().SetTimer(AppleSpawnTimerHandle, this, &ASCR_Gamemode::SpawnApple, AppleSpawnInterval, true);

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	ASCR_PlayerController* SCR_PC = nullptr;
	ASCR_Player* NewPawn = nullptr;

	if (PC)
	{
		SCR_PC = Cast<ASCR_PlayerController>(PC);
		if (SCR_PC && SCR_PC->YourSnakePawnClass)
		{
			APawn* OldPawn = PC->GetPawn();
			if (OldPawn)
			{
				OldPawn->Destroy();
			}

			FVector SpawnLocation(0.f, 0.f, 600.f);
			FRotator SpawnRotation = FRotator::ZeroRotator;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			NewPawn = GetWorld()->SpawnActor<ASCR_Player>(
				SCR_PC->YourSnakePawnClass,
				SpawnLocation,
				SpawnRotation,
				SpawnParams
			);

			if (NewPawn)
			{
				NewPawn->SetActorLocation(FVector(0.f, 0.f, 100.f));
				PC->Possess(NewPawn);
			}
		}
	}

	if (SCR_PC)
	{
		SCR_PC->ResetSecondPlayerSpawnFlag();
	}
}


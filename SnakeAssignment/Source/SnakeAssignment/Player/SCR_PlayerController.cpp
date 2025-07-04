#include "SCR_PlayerController.h"

#include "SnakeAssignment/Management/SCR_Gamemode.h"

void ASCR_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (!GetPawn())
	{
		UWorld* World = GetWorld();
		if (!World) return;


		for (TActorIterator<APawn> It(World); It; ++It)
		{
			APawn* FoundPawn = *It;

			if (FoundPawn->GetController()) continue;

			if (FoundPawn->IsA(YourSnakePawnClass))
			{
				Possess(FoundPawn);
				break;
			}
		}
	}
}

void ASCR_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetAuthGameMode<ASCR_Gamemode>()->CurrentGameState != EGameState::Game)
		return;

	const int32 MyID = UGameplayStatics::GetPlayerControllerID(this);

	if (!MyPlayer)
	{
		MyPlayer = Cast<ASCR_Player>(GetPawn());
		if (MyPlayer)
		{
			MyPlayer->playerNumber = MyID;
		}
	}
	
	
	if (!MyPawn && GetPawn()) MyPawn = GetPawn();


	if (!GetPawn()) return;
	
	switch (dir)
	{
		case EDir::up:
		GetPawn()->AddMovementInput(FVector::ForwardVector, 1); 
		break;
		
		case EDir::down:
		GetPawn()->AddMovementInput(FVector::ForwardVector, -1); 
		break;
		
		case EDir::right:
		GetPawn()->AddMovementInput(FVector::RightVector, 1); 
		break;
		
		case EDir::left:
		GetPawn()->AddMovementInput(FVector::RightVector, -1); 
		break;
		
		default:
		
		break;
	}
}

ASCR_PlayerController::ASCR_PlayerController()
{
	bAutoManageActiveCameraTarget = false;

	static ConstructorHelpers::FClassFinder<APawn> SnakePawnBPClass(TEXT("/Game/Blueprints/BP_Player"));
	if (SnakePawnBPClass.Succeeded())
	{
		YourSnakePawnClass = SnakePawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> SnakeBP(TEXT("/Game/Blueprints/BP_Player"));
	if (SnakeBP.Succeeded())
	{
		AISnakeClass = SnakeBP.Class;
	}

	AIControllerClass = ASCR_PlayerAIController::StaticClass();
}

void ASCR_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	check(InputComponent);

	const int32 PlayerID = UGameplayStatics::GetPlayerControllerID(this);
	if (PlayerID == 0)
	{
		InputComponent->BindAction("ChangeState", IE_Pressed, this, &ASCR_PlayerController::ChangeState);
		
		InputComponent->BindAxis("MoveForward_P1", this, &ASCR_PlayerController::HandleMoveForward);
		InputComponent->BindAxis("MoveRight_P1", this, &ASCR_PlayerController::HandleMoveRight);

		InputComponent->BindAction("SpawnSecondPlayer", IE_Pressed, this, &ASCR_PlayerController::TrySpawnSecondPlayer);
		
		InputComponent->BindAction("SpawnAIPlayer", IE_Pressed, this, &ASCR_PlayerController::TrySpawnAIPlayer);
		
		InputComponent->BindAxis("MoveForward_P2", this, &ASCR_PlayerController::SendMoveForward);
		InputComponent->BindAxis("MoveRight_P2", this, &ASCR_PlayerController::SendMoveRight);
	}
}

void ASCR_PlayerController::ChangeState()
{
	ASCR_Gamemode* GM = Cast<ASCR_Gamemode>(UGameplayStatics::GetGameMode(this));
	if (!GM) return;

	EGameState Current = GM->CurrentGameState;
	EGameState Next;

	switch (Current)
	{
	case EGameState::MainMenu:
		Next = EGameState::Game;
		break;

	case EGameState::Game:
		Next = EGameState::Outro;
		break;

	case EGameState::Outro:
	default:
		Next = EGameState::MainMenu;
		break;
	}

	GM->SetGameState(Next);
}

void ASCR_PlayerController::HandleMoveForward(float Value)
{
	ASCR_Gamemode* GM = Cast<ASCR_Gamemode>(UGameplayStatics::GetGameMode(this));
	if (!GM || GM->CurrentGameState != EGameState::Game) return;
	
	if (!GetPawn()) return;
	if (Value == 0.f) return;

	if (Value == 1) dir = EDir::up;
	else if (Value == -1) dir = EDir::down;
}

void ASCR_PlayerController::HandleMoveRight(float Value)
{
	ASCR_Gamemode* GM = Cast<ASCR_Gamemode>(UGameplayStatics::GetGameMode(this));
	if (!GM || GM->CurrentGameState != EGameState::Game) return;
	
	if (!GetPawn()) return;
	if (Value == 0.f) return;

	if (Value == 1) dir = EDir::right;
	else if (Value == -1) dir = EDir::left;
}

void ASCR_PlayerController::SendMoveForward(float Value)
{
	if (SecondPlayer) SecondPlayer->HandleMoveForward(Value);
}

void ASCR_PlayerController::SendMoveRight(float Value)
{
	if (SecondPlayer) SecondPlayer->HandleMoveRight(Value);
}

void ASCR_PlayerController::TrySpawnSecondPlayer()
{
	ASCR_Gamemode* GM = Cast<ASCR_Gamemode>(UGameplayStatics::GetGameMode(this));
	if (!GM || GM->CurrentGameState != EGameState::Game) return;
	if (bHasSpawnedSecondPlayer) return;

	UWorld* World = GetWorld();
	if (!World) return;

	if (UGameplayStatics::GetPlayerControllerID(this) == 0)
	{
		APlayerController* ExistingPC = UGameplayStatics::GetPlayerController(World, 1);

		if (!ExistingPC || !ExistingPC->GetPawn())
		{
			FVector SnakeSpawnLocation = FVector(300.f, 0.f, 100.f);
			FActorSpawnParameters SpawnParams;

			APawn* SpawnedPawn = World->SpawnActor<APawn>(YourSnakePawnClass, SnakeSpawnLocation, FRotator::ZeroRotator, SpawnParams);
			APlayerController* NewPC = ExistingPC ? ExistingPC : UGameplayStatics::CreatePlayer(World, 1, true);

			if (NewPC && SpawnedPawn)
			{
				NewPC->Possess(SpawnedPawn);
				ASCR_Player* NewPlayer = Cast<ASCR_Player>(SpawnedPawn);
				if (NewPlayer) NewPlayer->playerNumber = 1;
				bHasSpawnedSecondPlayer = true;
				SecondPlayer = Cast<ASCR_PlayerController>(NewPC);
			}
		}
	}
}

void ASCR_PlayerController::TrySpawnAIPlayer()
{
	ASCR_Gamemode* GM = Cast<ASCR_Gamemode>(UGameplayStatics::GetGameMode(this));
	if (!GM || GM->CurrentGameState != EGameState::Game) return;
	
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
}

void ASCR_PlayerController::ResetSecondPlayerSpawnFlag()
{
	bHasSpawnedSecondPlayer = false;
}

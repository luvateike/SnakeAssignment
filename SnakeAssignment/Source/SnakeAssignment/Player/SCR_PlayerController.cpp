#include "SCR_PlayerController.h"

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
}

void ASCR_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	check(InputComponent);

	const int32 PlayerID = UGameplayStatics::GetPlayerControllerID(this);
	if (PlayerID == 0)
	{
		InputComponent->BindAxis("MoveForward_P1", this, &ASCR_PlayerController::HandleMoveForward);
		InputComponent->BindAxis("MoveRight_P1", this, &ASCR_PlayerController::HandleMoveRight);

		InputComponent->BindAction("SpawnSecondPlayer", IE_Pressed, this, &ASCR_PlayerController::TrySpawnSecondPlayer);
		
		InputComponent->BindAxis("MoveForward_P2", this, &ASCR_PlayerController::SendMoveForward);
		InputComponent->BindAxis("MoveRight_P2", this, &ASCR_PlayerController::SendMoveRight);
	}
}

void ASCR_PlayerController::HandleMoveForward(float Value)
{
	if (!GetPawn()) return;
	if (Value == 0.f) return;

	if (Value == 1) dir = EDir::up;
	else if (Value == -1) dir = EDir::down;
}

void ASCR_PlayerController::HandleMoveRight(float Value)
{
	
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
	if (bHasSpawnedSecondPlayer) return;
	UWorld* World = GetWorld();
	if (!World) return;

	if (UGameplayStatics::GetPlayerControllerID(this) == 0)
	{
		if (UGameplayStatics::GetPlayerController(World, 1) == nullptr)
		{
			FVector SnakeSpawnLocation = FVector(300.f, 0.f, 100.f);
			
			FActorSpawnParameters SpawnParams;
			
			World->SpawnActor<APawn>(YourSnakePawnClass, SnakeSpawnLocation, FRotator::ZeroRotator, SpawnParams);

			APlayerController* NewPC = UGameplayStatics::CreatePlayer(World, 1, true);
			if (NewPC)
			{
				bHasSpawnedSecondPlayer = true;
				SecondPlayer = Cast<ASCR_PlayerController>(NewPC);
			}
		}
	}
}


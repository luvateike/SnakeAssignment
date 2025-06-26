#include "SCR_PlayerAIController.h"
#include "../Objects/SCR_Apple.h"
#include "Kismet/GameplayStatics.h"

ASCR_PlayerAIController::ASCR_PlayerAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASCR_PlayerAIController::BeginPlay()
{
	Super::BeginPlay();
	ControlledSnake = Cast<ASCR_Player>(GetPawn());
}

void ASCR_PlayerAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!ControlledSnake)
	{
		ControlledSnake = Cast<ASCR_Player>(GetPawn());
		if (!ControlledSnake) return;
	}
	MakeDecision();
}

void ASCR_PlayerAIController::MakeDecision()
{
	TArray<AActor*> Apples;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCR_Apple::StaticClass(), Apples);
	if (Apples.Num() == 0) return;

	FVector SnakeLocation = ControlledSnake->GetActorLocation();
	AActor* ClosestApple = nullptr;
	float MinDist = FLT_MAX;

	if (!ClosestApple)
	{
		for (AActor* Apple : Apples)
		{
			float Dist = FVector::Dist2D(SnakeLocation, Apple->GetActorLocation());
			if (Dist < MinDist)
			{
				MinDist = Dist;
				ClosestApple = Apple;
			}
		}
	}

	if (!ClosestApple) return;

	TargetPos = ClosestApple->GetActorLocation();
	FVector Direction = TargetPos - SnakeLocation;

	float DeltaX = Direction.X;
	float DeltaY = Direction.Y;

	UE_LOG(LogTemp, Warning, TEXT("AI T: X=%f, Y=%f, Z=%f"), TargetPos.X, TargetPos.Y, TargetPos.Z);
	
	UE_LOG(LogTemp, Warning, TEXT("AI S: X=%f, Y=%f, Z=%f"), SnakeLocation.X, SnakeLocation.Y, SnakeLocation.Z);
	
	UE_LOG(LogTemp, Warning, TEXT("AI D: X=%f, Y=%f, Z=%f"), Direction.X, Direction.Y, Direction.Z);

	if (FMath::Abs(DeltaX) > FMath::Abs(DeltaY))
	{
		if (TargetPos.X > SnakeLocation.X)
		{
			ControlledSnake->AddMovementInput(FVector::ForwardVector, 1);
		}
		else
		{
			ControlledSnake->AddMovementInput(FVector::ForwardVector, -1);
		}
	}
	else
	{
		if (TargetPos.Y > SnakeLocation.Y)
		{
			ControlledSnake->AddMovementInput(FVector::RightVector, 1);
		}
		else
		{
			ControlledSnake->AddMovementInput(FVector::RightVector, -1);
		}
	}
	
	// if (FMath::Abs(DeltaX) > FMath::Abs(DeltaY))
	// {
	// 	if (DeltaX > 0)
	// 	{
	// 		ControlledSnake->AddMovementInput(FVector::RightVector, 1);
	// 	}
	// 	else
	// 	{
	// 		ControlledSnake->AddMovementInput(FVector::RightVector, -1);
	// 	}
	// }
	// else
	// {
	// 	if (DeltaY > 0)
	// 	{
	// 		ControlledSnake->AddMovementInput(FVector::ForwardVector, 1);
	// 	}
	// 	else
	// 	{
	// 		ControlledSnake->AddMovementInput(FVector::ForwardVector, -1);
	// 	}
	// }
}

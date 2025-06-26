#include "SCR_PlayerAIController.h"
#include "../Objects/SCR_Apple.h"
#include "Kismet/GameplayStatics.h"
#include "SnakeAssignment/Management/SCR_Gamemode.h"

ASCR_PlayerAIController::ASCR_PlayerAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASCR_PlayerAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ASCR_PlayerAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ASCR_Gamemode* GM = Cast<ASCR_Gamemode>(UGameplayStatics::GetGameMode(this));
	if (!GM || GM->CurrentGameState != EGameState::Game) return;
	
	if (!ControlledSnake)
	{
		ControlledSnake = Cast<ASCR_Player>(GetPawn());
		ControlledSnake->bIsAI = true;
		ControlledSnake->OnDeath.AddDynamic(this, &ASCR_PlayerAIController::UnTargetClosestApple);
		if (!ControlledSnake) return;
	}
	MakeDecision();
}

void ASCR_PlayerAIController::MakeDecision()
{
	TArray<AActor*> Apples;
	
	FVector SnakeLocation = ControlledSnake->GetActorLocation();
	
	if (!ClosestApple || !IsValid(ClosestApple))
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCR_Apple::StaticClass(), Apples);
		if (Apples.Num() == 0) return;

		float MinDist = FLT_MAX;
		for (AActor* Apple : Apples)
		{
			float Dist = FVector::Dist2D(SnakeLocation, Apple->GetActorLocation());
			if (Dist < MinDist && !IsValid(Cast<ASCR_Apple>(Apple)->BeingTargetedBy))
			{
				MinDist = Dist;
				ClosestApple = Apple;
			}
		}
		
		if (ASCR_Apple* AppleClass = Cast<ASCR_Apple>(ClosestApple)) AppleClass->BeingTargetedBy = Cast<ASCR_Player>(GetPawn());
	}

	if (!ClosestApple || !IsValid(ClosestApple)) return;
	
	TargetPos = ClosestApple->GetActorLocation();
	FVector Direction = TargetPos - SnakeLocation;

	float DeltaX = Direction.X;
	float DeltaY = Direction.Y;

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
}

void ASCR_PlayerAIController::UnTargetClosestApple(AActor* DeadPlayer)
{
	if (ASCR_Apple* AppleClass = Cast<ASCR_Apple>(ClosestApple)) AppleClass->BeingTargetedBy = nullptr;
}
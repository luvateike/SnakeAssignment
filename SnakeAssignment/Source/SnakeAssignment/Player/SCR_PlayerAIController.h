// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "SCR_Player.h"
#include "SCR_PlayerAIController.generated.h"

UCLASS()
class SNAKEASSIGNMENT_API ASCR_PlayerAIController : public AController
{
	GENERATED_BODY()

public:
	ASCR_PlayerAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	ASCR_Player* ControlledSnake;
	FVector TargetPos;

	void MakeDecision();
	void UnTargetClosestApple(AActor* DeadPlayer);

	AActor* ClosestApple = nullptr;
};


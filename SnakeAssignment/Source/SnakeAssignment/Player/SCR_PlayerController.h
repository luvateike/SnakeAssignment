// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SCR_Player.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "EngineUtils.h"
#include "SCR_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SNAKEASSIGNMENT_API ASCR_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	void Tick(float DeltaTime);

public:
	ASCR_PlayerController();

	UPROPERTY(VisibleAnywhere)
	APawn* MyPawn;

	virtual void SetupInputComponent() override;

	UFUNCTION()
	void HandleMoveForward(float Value);

	UFUNCTION()
	void HandleMoveRight(float Value);
	
	UFUNCTION()
	void SendMoveForward(float Value);
	
	UFUNCTION()
	void SendMoveRight(float Value);

	UFUNCTION()
	void TrySpawnSecondPlayer();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASCR_Player> YourSnakePawnClass;

private:

	ASCR_PlayerController* SecondPlayer;
	
	bool bHasSpawnedSecondPlayer = false;

};

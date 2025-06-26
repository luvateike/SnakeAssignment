#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Player/SCR_PlayerController.h"
#include "../Player/SCR_Player.h"
#include "../Objects/SCR_Apple.h"
#include "SCR_Gamemode.generated.h"

UCLASS()
class SNAKEASSIGNMENT_API ASCR_Gamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASCR_Gamemode();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateUI();

	UFUNCTION()
	void GetLastScore(AActor* DeadPlayer);

	UPROPERTY(EditAnywhere, Category = "Apple Spawning")
	TSubclassOf<ASCR_Apple> AppleBlueprint;

	UPROPERTY(EditAnywhere, Category = "Apple Spawning")
	float BoundX = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Apple Spawning")
	float BoundY = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Apple Spawning")
	float AppleSpawnInterval = 2.0f;

	UPROPERTY(EditAnywhere, Category = "UI")
	float UICheckInterval = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Walls")
	TSubclassOf<AActor> WallBlueprint;

	UFUNCTION()
	void SpawnWalls();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ScoresDisplayClass;

	UPROPERTY()
	class USCR_ScoresDisplay* ScoresDisplayWidget;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	FString TotalScoresText = "Scores:\n";

	TArray<AActor*> RegisteredPlayerScores;

private:
	void SpawnApple();

	FTimerHandle AppleSpawnTimerHandle;
	
	FTimerHandle UserInterfaceTimerHandle;
};

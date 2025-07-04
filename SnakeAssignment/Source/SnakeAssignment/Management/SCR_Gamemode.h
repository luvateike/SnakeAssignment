#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "../Player/SCR_PlayerController.h"
#include "../Player/SCR_Player.h"
#include "../Objects/SCR_Apple.h"
#include "SCR_Gamemode.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	MainMenu,
	Game,
	Outro
};

UCLASS()
class SNAKEASSIGNMENT_API ASCR_Gamemode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASCR_Gamemode();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	EGameState CurrentGameState;

	UFUNCTION(BlueprintCallable, Category = "Game State")
	void SetGameState(EGameState NewState);

	FTimerHandle QuitGameTimerHandle;
	UFUNCTION()
	void QuitGameAfterOutro();

	int32 CurrentLevel = 0;
	const int32 MaxLevels = 3;

	UPROPERTY(EditAnywhere, Category = "Level Settings")
	TArray<float> LevelAppleSpawnIntervals;

	UPROPERTY(EditAnywhere, Category = "Level Settings")
	TArray<float> LevelBoundsX;

	UPROPERTY(EditAnywhere, Category = "Level Settings")
	TArray<float> LevelBoundsY;

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
	void AdvanceLevel();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuDisplay;
	UPROPERTY()
	class UUserWidget* MainMenuWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> OutroDisplay;
	UPROPERTY()
	class USCR_OutroScoreDisplay* OutroWidget;
	
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

	UPROPERTY()
	TArray<AActor*> Walls;
};

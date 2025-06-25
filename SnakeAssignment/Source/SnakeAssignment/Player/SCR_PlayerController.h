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

UENUM(BlueprintType)
enum class EDir : uint8
{
	up     UMETA(DisplayName = "up"),
	down   UMETA(DisplayName = "down"),
	left   UMETA(DisplayName = "left"),
	right  UMETA(DisplayName = "right")
};

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
	
	UPROPERTY(VisibleAnywhere)
	ASCR_PlayerController* SecondPlayer;

private:
	
	bool bHasSpawnedSecondPlayer = false;

	EDir dir = EDir::up;

};

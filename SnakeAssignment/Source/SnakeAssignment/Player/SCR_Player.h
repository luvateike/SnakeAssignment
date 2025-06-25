#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "SCR_Player.generated.h"

UCLASS()
class SNAKEASSIGNMENT_API ASCR_Player : public APawn
{
	GENERATED_BODY()

public:
	ASCR_Player();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 playerNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LastPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> TailBlueprint;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* MovementComponent;

};

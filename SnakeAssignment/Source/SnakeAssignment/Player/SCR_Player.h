#pragma once

#include "CoreMinimal.h"
#include "SCR_Tail.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SphereComponent.h"
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
	void SetPositions(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void AddTailSegment();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 playerNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASCR_Tail*> Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LastPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASCR_Tail> TailBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float timeToResetPosition = 0.5f;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* HeadCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAI = false;

	UFUNCTION()
	void OnHeadOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
					   bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable)
	void Die();


private:
	
	float resetPositionTimer = 0;

};

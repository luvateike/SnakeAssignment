#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCR_Tail.generated.h"

UCLASS()
class SNAKEASSIGNMENT_API ASCR_Tail : public AActor
{
	GENERATED_BODY()

public:
	ASCR_Tail();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* PlayerOwner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector lastPosition;
};

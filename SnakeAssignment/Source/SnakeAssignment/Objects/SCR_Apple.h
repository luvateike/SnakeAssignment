#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "SCR_Apple.generated.h"

UCLASS()
class SNAKEASSIGNMENT_API ASCR_Apple : public AActor
{
	GENERATED_BODY()

public:
	ASCR_Apple();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionSphere;
};

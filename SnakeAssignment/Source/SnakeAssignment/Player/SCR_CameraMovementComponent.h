#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "SCR_CameraMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SNAKEASSIGNMENT_API USCR_CameraMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USCR_CameraMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float Height = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	TArray<AActor*> CurrentPlayerActors;

	static FVector GetMiddlePoint(const TArray<AActor*>& PlayerActors);

private:
	FTimerHandle PlayerSearchTimerHandle;
	void RefreshPlayerList();
};

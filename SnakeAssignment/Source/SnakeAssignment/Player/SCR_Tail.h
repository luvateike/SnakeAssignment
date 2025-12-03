#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCR_Tail.generated.h"

class UPrimitiveComponent;
class UMeshComponent;
class ASCR_Player;


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



	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPrimitiveComponent* CollisionComp;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMeshComponent* MeshComp;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasSetMaterial = false;

	
	float OwnerCheckTimer = 0.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OwnerCheckInterval = 1.0f;


	UFUNCTION()
	void OnTailOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};

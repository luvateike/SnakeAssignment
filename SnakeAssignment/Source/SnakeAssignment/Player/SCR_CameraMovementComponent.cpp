#include "SCR_CameraMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "SCR_Player.h"


USCR_CameraMovementComponent::USCR_CameraMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USCR_CameraMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	RefreshPlayerList();
	
	GetOwner()->GetWorldTimerManager().SetTimer(
		   PlayerSearchTimerHandle,
		   this,
		   &USCR_CameraMovementComponent::RefreshPlayerList,
		   3.0f,
		   true
	   );
}


void USCR_CameraMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
												 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentPlayerActors.Num() > 0)
	{
		FVector MiddlePoint = GetMiddlePoint(CurrentPlayerActors);

		float MaxDistance = 0.f;
		for (int32 i = 0; i < CurrentPlayerActors.Num(); ++i)
		{
			for (int32 j = i + 1; j < CurrentPlayerActors.Num(); ++j)
			{
				if (IsValid(CurrentPlayerActors[i]) && IsValid(CurrentPlayerActors[j]))
				{
					float Distance = FVector::Dist(CurrentPlayerActors[i]->GetActorLocation(), CurrentPlayerActors[j]->GetActorLocation());
					MaxDistance = FMath::Max(MaxDistance, Distance+1000);
				}
			}
		}

		float DesiredHeight = FMath::Max(2000.0f, MaxDistance);
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		FVector TargetLocation = FVector(MiddlePoint.X, MiddlePoint.Y, DesiredHeight);
		float InterpSpeed = 2.5f; 
		FVector SmoothedLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
		GetOwner()->SetActorLocation(SmoothedLocation);
	}
}


FVector USCR_CameraMovementComponent::GetMiddlePoint(const TArray<AActor*>& PlayerActors)
{
	if (PlayerActors.Num() == 0)
	{
		return FVector::ZeroVector;
	}

	float SumX = 0.f;
	float SumY = 0.f;

	for (AActor* Actor : PlayerActors)
	{
		if (IsValid(Actor))
		{
			FVector Position = Actor->GetActorLocation();
			SumX += Position.X;
			SumY += Position.Y;
		}
	}
	
	return FVector(SumX / PlayerActors.Num(), SumY / PlayerActors.Num(), 0.0f );
}

void USCR_CameraMovementComponent::RefreshPlayerList()
{
	CurrentPlayerActors.Empty();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCR_Player::StaticClass(), CurrentPlayerActors);
}
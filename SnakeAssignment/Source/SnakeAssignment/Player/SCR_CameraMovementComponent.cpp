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
		FVector NewLocation = FVector(MiddlePoint.X, MiddlePoint.Y, Height);
		GetOwner()->SetActorLocation(NewLocation);
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
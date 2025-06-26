#include "SCR_Player.h"
#include "../Objects/SCR_Apple.h"
#include "ToolContextInterfaces.h"
#include "Engine/World.h"

ASCR_Player::ASCR_Player()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

	HeadCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollision"));
	HeadCollision->SetupAttachment(RootComponent);
	HeadCollision->SetSphereRadius(50.0f);
	HeadCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	HeadCollision->OnComponentBeginOverlap.AddDynamic(this, &ASCR_Player::OnHeadOverlap);

}

void ASCR_Player::BeginPlay()
{
	Super::BeginPlay();
	
	AddTailSegment();
}

void ASCR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetPositions(DeltaTime);

	for (int i = 0; i < Body.Num(); i++)
	{
		ASCR_Tail* tail = Body[i];

		FVector TargetLocation;

		if (i == 0)
		{
			TargetLocation = LastPosition;
		}
		else
		{
			TargetLocation = Body[i-1]->lastPosition;
		}
		
		FVector CurrentLocation = tail->GetActorLocation();

		if (!CurrentLocation.Equals(TargetLocation, 0.1f))
		{
			FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
			FVector NewLocation = CurrentLocation + Direction * Speed * DeltaTime;
			tail->SetActorLocation(NewLocation);
		}
	}
}

void ASCR_Player::SetPositions(float DeltaTime)
{
	if (resetPositionTimer > 0)
	{
		resetPositionTimer -= DeltaTime;
		return;
	}

	LastPosition = GetActorLocation();

	for (ASCR_Tail* tail : Body)
	{
		tail->lastPosition = tail->GetActorLocation();
	}

	resetPositionTimer = timeToResetPosition;
}

void ASCR_Player::AddTailSegment()
{
	if (!TailBlueprint) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector SpawnLocation = LastPosition;
	if (Body.Num() > 0)
	{
		SpawnLocation = Body.Last()->lastPosition;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ASCR_Tail* NewTail = World->SpawnActor<ASCR_Tail>(TailBlueprint, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	if (NewTail)
	{
		NewTail->SetActorLocation(SpawnLocation);
		Body.Add(NewTail);
	}
}

void ASCR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASCR_Player::OnHeadOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA<ASCR_Apple>())
	{
		OtherActor->Destroy();
		AddTailSegment();
	}
}

void ASCR_Player::Die()
{
	for (ASCR_Tail* Segment : Body)
	{
		if (IsValid(Segment))
		{
			Segment->Destroy();
		}
	}
	Body.Empty();
	Destroy();
}

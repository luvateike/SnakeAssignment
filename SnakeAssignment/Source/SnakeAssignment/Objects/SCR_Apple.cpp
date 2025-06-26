#include "SCR_Apple.h"


ASCR_Apple::ASCR_Apple()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetSphereRadius(50.0f);
	CollisionSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ASCR_Apple::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCR_Apple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


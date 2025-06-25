#include "SCR_Tail.h"


ASCR_Tail::ASCR_Tail()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASCR_Tail::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCR_Tail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


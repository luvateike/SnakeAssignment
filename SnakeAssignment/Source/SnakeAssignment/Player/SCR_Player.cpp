#include "SCR_Player.h"


ASCR_Player::ASCR_Player()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));

}

void ASCR_Player::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCR_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASCR_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


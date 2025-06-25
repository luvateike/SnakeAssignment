// Fill out your copyright notice in the Description page of Project Settings.


#include "SCR_Apple.h"


// Sets default values
ASCR_Apple::ASCR_Apple()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASCR_Apple::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCR_Apple::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


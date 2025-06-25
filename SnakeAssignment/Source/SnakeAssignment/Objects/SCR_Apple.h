// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SCR_Apple.generated.h"

UCLASS()
class SNAKEASSIGNMENT_API ASCR_Apple : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASCR_Apple();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

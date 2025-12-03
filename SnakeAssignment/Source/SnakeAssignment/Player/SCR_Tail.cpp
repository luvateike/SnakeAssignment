#include "SCR_Tail.h"
#include "SCR_Player.h"

#include "Components/PrimitiveComponent.h"
#include "Components/MeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

ASCR_Tail::ASCR_Tail()
{
	PrimaryActorTick.bCanEverTick = true;

	
	CollisionComp = nullptr;
	MeshComp      = nullptr;
}

void ASCR_Tail::BeginPlay()
{
	Super::BeginPlay();

	Super::BeginPlay();


	CollisionComp = Cast<UPrimitiveComponent>(
		GetComponentByClass(UPrimitiveComponent::StaticClass())
	);

	if (CollisionComp)
	{
		CollisionComp->SetGenerateOverlapEvents(true);
		CollisionComp->OnComponentBeginOverlap.AddDynamic(
			this, &ASCR_Tail::OnTailOverlap
		);
	}


	MeshComp = Cast<UMeshComponent>(
		GetComponentByClass(UMeshComponent::StaticClass())
	);
}

void ASCR_Tail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	OwnerCheckTimer += DeltaTime;
	if (OwnerCheckTimer >= OwnerCheckInterval)
	{
		OwnerCheckTimer = 0.f;

		if (!IsValid(PlayerOwner))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 2.f, FColor::Red,
					TEXT("Tail: PlayerOwner invalid, destroying tail")
				);
			}

			Destroy();
			return;
		}
	}

	
	if (!bHasSetMaterial && IsValid(PlayerOwner) && MeshComp)
	{
		ASCR_Player* OwningPlayer = Cast<ASCR_Player>(PlayerOwner);
		if (OwningPlayer)
		{
		
			UMeshComponent* PlayerMesh = Cast<UMeshComponent>(
				OwningPlayer->GetComponentByClass(UMeshComponent::StaticClass())
			);

			if (PlayerMesh)
			{
				UMaterialInterface* OwnerMat = PlayerMesh->GetMaterial(0);
				if (OwnerMat)
				{
					MeshComp->SetMaterial(0, OwnerMat);
					bHasSetMaterial = true;

					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(
							-1, 2.f, FColor::Green,
							TEXT("Tail: Copied material from owning player")
						);
					}
				}
			}
		}
	}
}

void ASCR_Tail::OnTailOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor || OtherActor == this)
	{
		return;
	}

	ASCR_Player* HitPlayer      = Cast<ASCR_Player>(OtherActor);
	ASCR_Player* OwnerAsPlayer  = Cast<ASCR_Player>(PlayerOwner);

	
	if (HitPlayer && HitPlayer != OwnerAsPlayer)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1, 2.f, FColor::Yellow,
				TEXT("Tail: Hit another player, calling Die()")
			);
		}

		HitPlayer->Die();
	}
}

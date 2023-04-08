// DAE - Group 15 - SquirrelMania
#include "KillVolume.h"
#include "ParallaxCamera.h"
#include "PlatformBase.h"
#include "SquirrelCharacter.h"
#include "SquirrelManiaGameModeBase.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AKillVolume::AKillVolume()
{
	PrimaryActorTick.bCanEverTick = true;

	m_pCollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	m_pCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AKillVolume::OnOverlapBegin);
}

void AKillVolume::BeginPlay()
{
	Super::BeginPlay();

	m_pCamera = UGameplayStatics::GetActorOfClass(GetWorld(), AParallaxCamera::StaticClass());
	if (m_pCamera == nullptr
		&& GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			FString::Printf(TEXT("Output: %s"), *FString{"KILLVOLUME BEGINPLAY - CAMERA NOT FOUND"}));
	}
}

void AKillVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASquirrelCharacter* pPlayer = Cast<ASquirrelCharacter>(OtherActor);
	if (pPlayer)
	{
		--pPlayer->m_Lives;

		FVector location = m_pCamera->GetActorLocation();
		constexpr float distanceCameraToPlatformsX = 100.f;
		constexpr float spawningHeightZ = 600.f;
		location.X += distanceCameraToPlatformsX;
		location.Z += spawningHeightZ;
		bool isSpawnedCorrectly = false;
		while (isSpawnedCorrectly == false)
		{
			const float randomLocationY = FMath::RandRange(-800, 800);
			location.Y += randomLocationY;

			TArray<FHitResult> hitResults;
			FVector traceStart = location;
			FVector traceEnd = location;
			traceEnd.Z -= (location.Z - GetActorLocation().Z);

			GetWorld()->LineTraceMultiByChannel(hitResults, traceStart, traceEnd, ECC_Visibility);

			for (auto HitResult : hitResults)
			{
				if(Cast<APlatformBase>(HitResult.GetActor()))
				{
					isSpawnedCorrectly = true;
				}
			}
		}

		pPlayer->SetActorLocation(location, false, nullptr, ETeleportType::ResetPhysics);
		//ASquirrelCharacter* pCharacter = Cast<ASquirrelCharacter>(pPlayer);
		pPlayer->GetCharacterMovement()->ClearAccumulatedForces();
		//Charge Rest
		pPlayer->DestroyChargeParticles();
		pPlayer->m_bCharging = false;
		pPlayer->m_bHeavyReset = true;
		


		Cast<ASquirrelManiaGameModeBase>(GetWorld()->GetAuthGameMode())->CheckVictoryCondition();
	}
}

void AKillVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_pCamera)
		return;

	FVector newLoc = m_pCamera->GetActorLocation();
	newLoc += m_CameraOffset;
	SetActorLocation(newLoc);
}
// DAE - Group 15 - SquirrelMania
#include "SlipperyPlatform.h"

#include <string>

#include "Components/BoxComponent.h"
#include "SquirrelCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

ASlipperyPlatform::ASlipperyPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	m_pCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_pCollisionBox->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	m_pCollisionBox->SetCollisionProfileName("Trigger");

	m_pCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASlipperyPlatform::OnOverlapBegin);
	m_pCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASlipperyPlatform::OnOverlapEnd);

	m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = m_pStaticMesh;
	m_pCollisionBox->SetupAttachment(GetRootComponent());
}

void ASlipperyPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if(ASquirrelCharacter* pPlayer = Cast<ASquirrelCharacter>(OtherActor))
	{
		m_pMovementComponent = Cast<UCharacterMovementComponent>(pPlayer->GetMovementComponent());

		if(m_pMovementComponent->GroundFriction != 0 && m_pMovementComponent->BrakingDecelerationWalking != 0)
		{
			m_PlayerGroundFiction = m_pMovementComponent->GroundFriction;
			m_PlayerBreakingDecelaration = m_pMovementComponent->BrakingDecelerationWalking;
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, std::to_string(m_PlayerGroundFiction).c_str());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, std::to_string(m_PlayerBreakingDecelaration).c_str());
		m_pMovementComponent->GroundFriction = 0;
		m_pMovementComponent->BrakingDecelerationWalking = 0;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, std::to_string(m_pMovementComponent->GroundFriction).c_str());
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, std::to_string(m_pMovementComponent->BrakingDecelerationWalking).c_str());
	}*/


	if (ASquirrelCharacter* player = Cast<ASquirrelCharacter>(OtherActor))
	{
		m_PlayerInAreaCount++;
	
	}
}

void ASlipperyPlatform::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//if (ASquirrelCharacter* pPlayer = Cast<ASquirrelCharacter>(OtherActor))
	//{
	//	if (m_pMovementComponent)
	//	{
	//		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, std::to_string(m_PlayerGroundFiction).c_str());
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("OverlapEnd"));
	//		m_pMovementComponent->GroundFriction = m_PlayerGroundFiction;
	//		m_pMovementComponent->BrakingDecelerationWalking = m_PlayerBreakingDecelaration;
	//		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,std::to_string(m_pMovementComponent->GroundFriction).c_str());
	//		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, std::to_string(m_pMovementComponent->BrakingDecelerationWalking).c_str());
	//	}
	//}
	if (ASquirrelCharacter* player = Cast<ASquirrelCharacter>(OtherActor))
	{
		m_PlayerInAreaCount--;
		UCharacterMovementComponent* movementComponent = Cast<UCharacterMovementComponent>(player->GetMovementComponent());
		movementComponent->GroundFriction = 3;
		movementComponent->BrakingDecelerationWalking = 2048;
	}
}

void ASlipperyPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (m_PlayerInAreaCount > 0)
	{
		TArray<AActor*> result;
		TSubclassOf<ASquirrelCharacter> filter;
		GetOverlappingActors(result, filter);
		for (auto actor : result)
		{
			ASquirrelCharacter* c = Cast<ASquirrelCharacter>(actor);
			if (!c)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
						FString::Printf(TEXT("Output: %s"), *FString{ "SLIPPERPLATFORM TICK - NO SQUIRRELCHARACTER" }));
				}
				return;
			}
			UCharacterMovementComponent* movementComponent = Cast<UCharacterMovementComponent>(c->GetMovementComponent());
			if(!movementComponent)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
					                                 FString::Printf(TEXT("Output: %s"), *FString{"SLIPPERPLATFORM TICK - NO MOVEMENT COMPONENT"}));
				}
				return;
			}

			if (c->GetMovementComponent()->Velocity.Z == 0)
			{
				movementComponent->GroundFriction = 0;
				movementComponent->BrakingDecelerationWalking = 0;
			} else
			{
				movementComponent->GroundFriction = 3;
				movementComponent->BrakingDecelerationWalking = 2048;
			}
		}
	}
}

void ASlipperyPlatform::BeginPlay()
{
	Super::BeginPlay();
}

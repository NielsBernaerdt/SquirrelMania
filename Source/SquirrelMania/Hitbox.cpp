// DAE - Group 15 - SquirrelMania
#include "Hitbox.h"
#include "PlatformBase.h"
#include "Components/BoxComponent.h"
AHitbox::AHitbox()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(m_pCollisionBox);
	m_pCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitboxDisplay"));
}
void AHitbox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(m_HitboxType == EHitboxEnum::GROUNDSTRIKEBOX)
	{
		//UPDATE POS//
		FVector newPos = m_pOwningPlayer->GetActorLocation();
		newPos.Z -= 100;
		SetActorLocation(newPos);

		//CHECK FOR COLLISION WITH PLATFORM
		TArray<AActor*> pOverlappingPlatforms;
		GetOverlappingActors(pOverlappingPlatforms, APlatformBase::StaticClass());
		if (pOverlappingPlatforms.Num() > 0)
		{
			Destroy();
		}

		//CHECK FOR COLLISION WITH PLAYER
		TArray<AActor*> pOverlapping;
		GetOverlappingActors(pOverlapping, ASquirrelCharacter::StaticClass());
		if (pOverlapping.Num() > 0)
		{
			for (const auto pActor : pOverlapping)
			{
				ASquirrelCharacter* pCharacter = Cast<ASquirrelCharacter>(pActor);
				FVector loc = pActor->GetActorLocation() - GetActorLocation(); //THIS LINE DIFFERENT FROM CHECKCOLLSION FUNCTION
				loc.Z = 10.f;
				loc.X = 0.f;
				loc.Normalize();
				loc *= m_KnockbackRange;
				if (pCharacter)
				{
					if (pCharacter != m_pOwningPlayer && GEngine)
					{
						if (pCharacter->m_bArmor == false)
						{
							pCharacter->LaunchCharacter(loc, true, true);
						}
						else
						{
							pCharacter->DestroyArmor();
						}
					}
				}
			}
		}
	}
}

void AHitbox::CheckCollision()
{
	TArray<AActor*> pOverlapping;
	GetOverlappingActors(pOverlapping, ASquirrelCharacter::StaticClass());
	if(pOverlapping.Num() > 0)
	{
		for(const auto pActor : pOverlapping)
		{
			ASquirrelCharacter* pCharacter = Cast<ASquirrelCharacter>(pActor);
			FVector loc = pActor->GetActorLocation() - m_pOwningPlayer->GetActorLocation();
			loc.Z += 10.f;
			loc.Normalize();
			loc *= m_KnockbackRange;
			if(pCharacter)
			{
				if (pCharacter == m_pOwningPlayer && GEngine)
					return;

				if(pCharacter->m_bArmor == false)
				{
					pCharacter->LaunchCharacter(loc, true,true);
					pCharacter->StunCharacter();
				} else
				{
					pCharacter->DestroyArmor();
				}
			}
		}
	}
}

void AHitbox::SpawnHitbox(AActor* owningPlayer)
{
	if(owningPlayer)
	{
		m_pOwningPlayer = owningPlayer;
	}
	switch(m_HitboxType)
	{
		case EHitboxEnum::ACTORBOX:
			break;
		case EHitboxEnum::PROXYBOX:
			break;
		case EHitboxEnum::STRIKEBOX:
			m_pCollisionBox->SetWorldLocation(m_HitboxLocation);
			m_pCollisionBox->AddWorldRotation(m_HitBoxRotation);
			CheckCollision();
			break;
		case EHitboxEnum::GROUNDSTRIKEBOX:
			m_pCollisionBox->SetWorldLocation(m_HitboxLocation);
			m_pCollisionBox->AddWorldRotation(m_HitBoxRotation);
			break;
	}
}
// DAE - Group 15 - SquirrelMania
#include "FallingPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "SquirrelCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

AFallingPlatform::AFallingPlatform()
{
	m_pCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_pCollisionBox->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	m_pCollisionBox->SetCollisionProfileName("Trigger");

	m_pCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFallingPlatform::OnOverlapBegin);
	m_pCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AFallingPlatform::OnOverlapEnd);

	m_pCollisionBoxBottom = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentBottom"));
	m_pCollisionBoxBottom->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	m_pCollisionBoxBottom->SetCollisionProfileName("Trigger");

	m_pCollisionBoxBottom->OnComponentBeginOverlap.AddDynamic(this, &AFallingPlatform::OnOverlapBeginBottom);

	m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = m_pStaticMesh;
	m_pCollisionBox->SetupAttachment(GetRootComponent());
	m_pCollisionBoxBottom->SetupAttachment(GetRootComponent());
}
void AFallingPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASquirrelCharacter* player = Cast<ASquirrelCharacter>(OtherActor))
	{
		m_PlayerInAreaCount++;
		/*Rumble();
		GetWorld()->GetTimerManager().SetTimer(m_FallTimer, this, &AFallingPlatform::LetPlatformFall, 1, false);*/
	}
}
void AFallingPlatform::OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(m_IsFalling)
	{
		if(ASquirrelCharacter* pPlayer = Cast<ASquirrelCharacter>(OtherActor))
		{

			if (pPlayer->m_bArmor == false)
			{
				if(pPlayer->GetActorLocation().Y >= GetActorLocation().Y)
				{
					pPlayer->LaunchCharacter(FVector(0, 3000, 0), true, true);
					
				} else
				{
					pPlayer->LaunchCharacter(FVector(0, -3000, 0), true, true);

				}
			}else
			{
				pPlayer->DestroyArmor();
			}
		}
	}
}
void AFallingPlatform::LetPlatformFall()
{
	m_IsFalling = true;
	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, 0));
	m_pStaticMesh->SetSimulatePhysics(true);
	m_pStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AFallingPlatform::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ASquirrelCharacter>(OtherActor))
	{
		m_PlayerInAreaCount--;
	}
}

void AFallingPlatform::Tick(float DeltaSeconds)
{
	if(!m_bIsRumbling && !m_IsFalling)
	{
		if(m_PlayerInAreaCount > 0)
		{
			TArray<AActor*> result;
			TSubclassOf<ASquirrelCharacter> filter;
			GetOverlappingActors(result, filter);
			for (auto actor : result)
			{
				ASquirrelCharacter* c = Cast<ASquirrelCharacter>(actor);
				if(c)
				{
					if(c->GetMovementComponent()->Velocity.Z == 0)
					{
						m_bIsRumbling = true;
						Rumble();
						GetWorld()->GetTimerManager().SetTimer(m_FallTimer, this, &AFallingPlatform::LetPlatformFall, 1, false);
					}
				}
			}
		}
		
	}

	if(m_IsFalling)
	{
		SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, 0));

	}
}

void AFallingPlatform::Rumble()
{
	
	FRotator newRot = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, FMath::RandRange(-m_MaxAngleRumbling, m_MaxAngleRumbling));
	//FMath::RInterpConstantTo(GetActorRotation(), newRot, GetWorld()->GetDeltaSeconds(), 500);
	int interpSpeed = 500;
	SetActorRotation(FMath::RInterpConstantTo(GetActorRotation(), newRot, GetWorld()->GetDeltaSeconds(), interpSpeed));
	if(m_IsFalling == false)
	{
		GetWorld()->GetTimerManager().SetTimer(m_RumbleTimer, this, &AFallingPlatform::Rumble, 0.05f, false);
	}
}

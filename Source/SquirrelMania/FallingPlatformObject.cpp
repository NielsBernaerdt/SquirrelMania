// DAE - Group 15 - SquirrelMania
#include "FallingPlatformObject.h"
#include "Components/BoxComponent.h"
#include "SquirrelCharacter.h"

AFallingPlatformObject::AFallingPlatformObject()
{
	PrimaryActorTick.bCanEverTick = true;

	m_pCollisionBoxBottom = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponentBottom"));
	m_pCollisionBoxBottom->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	m_pCollisionBoxBottom->SetCollisionProfileName("Trigger");

	m_pCollisionBoxBottom->OnComponentBeginOverlap.AddDynamic(this, &AFallingPlatformObject::OnOverlapBeginBottom);

	m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = m_pStaticMesh;
	m_pCollisionBoxBottom->SetupAttachment(GetRootComponent());
}
void AFallingPlatformObject::OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (m_IsFalling)
	{
		float LaunchInY = 3000;
		if (ASquirrelCharacter* pPlayer = Cast<ASquirrelCharacter>(OtherActor))
		{
			if (pPlayer->GetActorLocation().Y >= GetActorLocation().Y)
			{
				pPlayer->LaunchCharacter(FVector(0, LaunchInY, 0), true, true);
			}
			else
			{
				pPlayer->LaunchCharacter(FVector(0, -LaunchInY, 0), true, true);
			}
		}
	}
}
void AFallingPlatformObject::LetPlatformFall()
{
	m_IsFalling = true;
	m_pStaticMesh->SetSimulatePhysics(true);
	m_pStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}
void AFallingPlatformObject::BeginPlay()
{
	Super::BeginPlay();
	LetPlatformFall();
}
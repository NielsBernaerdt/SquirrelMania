// DAE - Group 15 - SquirrelMania
#include "ThickPlatform.h"
#include "SquirrelCharacter.h"
#include "Components/BoxComponent.h"

AThickPlatform::AThickPlatform()
{
	m_pCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_pCollisionBox->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	m_pCollisionBox->SetCollisionProfileName("Trigger");

	m_pCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AThickPlatform::OnOverlapBegin);

	m_pStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = m_pStaticMesh;
	m_pCollisionBox->SetupAttachment(GetRootComponent());
}

void AThickPlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASquirrelCharacter* pPlayer = Cast<ASquirrelCharacter>(OtherActor))
	{
		pPlayer->Bonk();
	}
}
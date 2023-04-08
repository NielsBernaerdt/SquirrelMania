// DAE - Group 15 - SquirrelMania
#include "Jetpack.h"
#include "SquirrelCharacter.h"
#include "Components/BoxComponent.h"

AJetpack::AJetpack()
{
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(m_pCollisionBox);
	m_pCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_pCollisionBox->SetBoxExtent(FVector(32.f, 32.f, 32.f));
	m_pCollisionBox->SetCollisionProfileName("Trigger");

	m_pCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AJetpack::OnOverlapBegin);
}
void AJetpack::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASquirrelCharacter* pPlayer = Cast<ASquirrelCharacter>(OtherActor))
	{
		pPlayer->m_bHasJetpack = true;
		AJetpack::Destroy();
	}
}
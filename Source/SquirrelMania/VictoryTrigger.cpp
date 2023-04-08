// DAE - Group 15 - SquirrelMania
#include "VictoryTrigger.h"
#include "ParallaxCamera.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AVictoryTrigger::AVictoryTrigger()
{
	USceneComponent* pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(pRoot);

	m_pCollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	m_pCollisionBox->SetupAttachment(pRoot);
}

void AVictoryTrigger::BeginPlay()
{
	Super::BeginPlay();

	m_pCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AVictoryTrigger::OnBoxBeginOverlap);
}

void AVictoryTrigger::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Cast<AParallaxCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), AParallaxCamera::StaticClass()))->EndGameReached();
}
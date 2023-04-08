// DAE - Group 15 - SquirrelMania
#include "Boundary.h"
#include "ParallaxCamera.h"
#include "Kismet/GameplayStatics.h"

ABoundary::ABoundary()
{
	PrimaryActorTick.bCanEverTick = true;
}
void ABoundary::BeginPlay()
{
	Super::BeginPlay();

	m_pCamera = UGameplayStatics::GetActorOfClass(GetWorld(), AParallaxCamera::StaticClass());
	if (m_pCamera == nullptr
		&& GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			FString::Printf(TEXT("Output: %s"), *FString{ "BOUNDARY BEGINPLAY - CAMERA NOT FOUND" }));
	}
}
void ABoundary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_pCamera)
		return;

	FVector newLoc = m_pCamera->GetActorLocation();
	newLoc += m_CameraOffset;
	SetActorLocation(newLoc);
}
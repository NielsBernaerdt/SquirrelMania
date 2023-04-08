// DAE - Group 15 - SquirrelMania
#include "ParallaxCamera.h"
#include "Boundary.h"
#include "ObjectSpawner.h"
#include "SquirrelController.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
AParallaxCamera::AParallaxCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	m_pRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(m_pRoot);

	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	m_pCamera->SetupAttachment(m_pRoot);
	m_pCamera->SetRelativeLocation(FVector{1130.f, 0.f, 90.f});
	m_pCamera->SetRelativeRotation(FRotator{ 0.f, 180.f, 0.f });
}

void AParallaxCamera::BeginPlay()
{
	Super::BeginPlay();

	const auto pController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (pController)
	{
		pController->SetViewTargetWithBlend(this);
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Output: %s"), *FString{ "PARALLAXCAMERA BEGINPLAY - CONTROLLER NOT FOUND" }));
	}
}

void AParallaxCamera::EndGameReached()
{
	TArray<AActor*> pBoundaries;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABoundary::StaticClass(), pBoundaries);
	for (auto pBoundary : pBoundaries)
	{
		pBoundary->Destroy();
	}
	TArray<AActor*> pObjectSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjectSpawner::StaticClass(), pObjectSpawners);
	for (auto pSpawner : pObjectSpawners)
	{
		pSpawner->Destroy();
	}
}
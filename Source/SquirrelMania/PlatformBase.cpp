// DAE - Group 15 - SquirrelMania
#include "PlatformBase.h"

APlatformBase::APlatformBase()
{
	PrimaryActorTick.bCanEverTick = true;
}
void APlatformBase::BeginPlay()
{
	Super::BeginPlay();
}

void APlatformBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlatformBase.generated.h"

UCLASS()
class SQUIRRELMANIA_API APlatformBase : public AActor
{
	GENERATED_BODY()
public:	
	APlatformBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
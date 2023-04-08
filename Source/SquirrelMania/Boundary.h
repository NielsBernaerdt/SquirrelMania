// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boundary.generated.h"

UCLASS()
class SQUIRRELMANIA_API ABoundary : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "ParallaxCamera")
	FVector m_CameraOffset = { 0.f, 0.f, 0.f };

	ABoundary();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	AActor* m_pCamera = nullptr;
};
// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillVolume.generated.h"

class UBoxComponent;
UCLASS()
class SQUIRRELMANIA_API AKillVolume : public AActor
{
	GENERATED_BODY()
public:
	AKillVolume();

	UPROPERTY(EditAnywhere, Category = "Construction")
		UBoxComponent* m_pCollisionBox;
	UPROPERTY(EditAnywhere, Category = "Construction")
		FVector m_CameraOffset = {0.f, 0.f, 0.f};

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	AActor* m_pCamera = nullptr;
};
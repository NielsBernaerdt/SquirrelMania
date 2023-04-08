// DAE - Group 15 - SquirrelMani
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallingPlatform.h"
#include "Jetpack.h"
#include "ArmorPickup.h"
#include "ObjectSpawner.generated.h"

UCLASS()
class SQUIRRELMANIA_API AObjectSpawner : public AActor
{
	GENERATED_BODY()
public:
	AObjectSpawner();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Offset Z")
	float m_OffsetZ = 100;
	float m_OffsetYMin = -1050;
	float m_OffsetYMax = 720;
	float m_OffsetZJetpackMin = 350;
	float m_OffsetZJetpackMax = 600;
	FVector m_BoxSize = FVector(220, 220, 220);
protected:
	virtual void BeginPlay() override;
	void SpawnPlatform();
	void SpawnJetpack();
	void SpawnArmor();
	void WaitPlatform();
	void InvisPlatform();

private:
	FTimerHandle m_FallingPlatformTimer;
	FTimerHandle m_FallingPlatformWaitTimer;
	FTimerHandle m_FallingPlatformInvisTimer;
	TSubclassOf<AFallingPlatform> m_FallingPlatformRef;

	FTimerHandle m_JetpackTimer;
	TSubclassOf<AJetpack> m_JetpackRef;

	FTimerHandle m_ArmorTimer;
	TSubclassOf<AArmorPickup> m_ArmorRef;

	FVector m_WorldLoc;
	FVector m_WorldDir;
	FVector m_ViewportSize;
	APlayerController* m_pController;

	AFallingPlatform* m_pFallingPlatform;

	bool m_bIsWaiting;
};
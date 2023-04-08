// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "PlatformBase.h"
#include "FallingPlatform.generated.h"

class UBoxComponent;

UCLASS()
class SQUIRRELMANIA_API AFallingPlatform : public APlatformBase
{
	GENERATED_BODY()
public:
	AFallingPlatform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction")
		UBoxComponent* m_pCollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction")
		UBoxComponent* m_pCollisionBoxBottom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construction")
		UStaticMeshComponent* m_pStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_MaxAngleRumbling = 1.5f;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void LetPlatformFall();

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Tick(float DeltaSeconds) override;

	
private:
	bool m_IsFalling;
	bool m_bIsRumbling;
	int m_PlayerInAreaCount;
	FTimerHandle m_FallTimer;
	FTimerHandle m_RumbleTimer;

	void Rumble();
};
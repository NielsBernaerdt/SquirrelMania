// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "PlatformBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SlipperyPlatform.generated.h"

class UBoxComponent;

UCLASS()
class SQUIRRELMANIA_API ASlipperyPlatform : public APlatformBase
{
	GENERATED_BODY()
public:
	ASlipperyPlatform();

	UPROPERTY(EditAnywhere, Category = "Construction")
		UBoxComponent* m_pCollisionBox;
	UPROPERTY(EditAnywhere, Category = "Construction")
		UStaticMeshComponent* m_pStaticMesh;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;

private:
	float m_PlayerBreakingDecelaration;
	float m_PlayerGroundFiction;
	int m_PlayerInAreaCount;
	UCharacterMovementComponent* m_pMovementComponent;
};
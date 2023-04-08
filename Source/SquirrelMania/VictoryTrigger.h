// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VictoryTrigger.generated.h"

class UBoxComponent;
UCLASS()
class SQUIRRELMANIA_API AVictoryTrigger : public AActor
{
	GENERATED_BODY()
public:	
	AVictoryTrigger();

	UPROPERTY(EditAnywhere, Category = "Construction")
		UBoxComponent* m_pCollisionBox;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
};
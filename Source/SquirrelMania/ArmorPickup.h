// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmorPickup.generated.h"

UCLASS()
class SQUIRRELMANIA_API AArmorPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AArmorPickup();

	UPROPERTY(EditAnywhere, Category = "Construction")
		class UBoxComponent* m_pCollisionBox;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
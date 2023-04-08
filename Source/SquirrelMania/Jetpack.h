// DAE - Group 15 - SquirrelMania

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Jetpack.generated.h"

class UBoxComponent;

UCLASS()
class SQUIRRELMANIA_API AJetpack : public AActor
{
	GENERATED_BODY()
public:	
	AJetpack();

	UPROPERTY(EditAnywhere, Category = "Construction")
		UStaticMeshComponent* m_pStaticMesh;

	UPROPERTY(EditAnywhere, Category = "Construction")
		UBoxComponent* m_pCollisionBox;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
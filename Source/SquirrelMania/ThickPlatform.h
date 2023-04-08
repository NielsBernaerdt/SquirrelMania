// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "PlatformBase.h"
#include "ThickPlatform.generated.h"

class UBoxComponent;
UCLASS()
class SQUIRRELMANIA_API AThickPlatform : public APlatformBase
{
	GENERATED_BODY()
public:
	AThickPlatform();

	UPROPERTY(EditAnywhere, Category = "Construction")
		UBoxComponent* m_pCollisionBox;

	UPROPERTY(EditAnywhere, Category = "Construction")
		UStaticMeshComponent* m_pStaticMesh;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
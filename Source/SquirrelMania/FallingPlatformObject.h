// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "PlatformBase.h"
#include "FallingPlatformObject.generated.h"

class UBoxComponent;

UCLASS()
class SQUIRRELMANIA_API AFallingPlatformObject : public APlatformBase
{
	GENERATED_BODY()
public:
	AFallingPlatformObject();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* m_pCollisionBoxBottom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pStaticMesh;

	UFUNCTION()
		void OnOverlapBeginBottom(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		void LetPlatformFall();
protected:
	virtual void BeginPlay() override;
private:
	bool m_IsFalling;
};
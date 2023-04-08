// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParallaxCamera.generated.h"

class UCameraComponent;

UCLASS()
class SQUIRRELMANIA_API AParallaxCamera : public AActor
{
	GENERATED_BODY()
public:	
	AParallaxCamera();
	virtual void BeginPlay() override;
	void EndGameReached();

	UPROPERTY(EditAnywhere, Category = "Construction")
	UCameraComponent* m_pCamera;
	UPROPERTY(VisibleAnywhere, Category = "Construction")
	USceneComponent* m_pRoot;

private:
	TArray<AActor*> m_pPlayers;
};
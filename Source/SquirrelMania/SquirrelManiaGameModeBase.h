// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SquirrelManiaGameModeBase.generated.h"

class AInputReceiver;

UCLASS()
class SQUIRRELMANIA_API ASquirrelManiaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	ASquirrelManiaGameModeBase();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, Category = "Game Activation");
	bool m_bIsGameActive = true;

	void StartGame();
	void StopGame();

	void CheckVictoryCondition();
	void SetPlayerReferences();

private:
	TSubclassOf<AInputReceiver> m_InputReceiverRef;
	TArray<AActor*> m_pPlayers;

	bool m_bStartGameOnce = false;
};
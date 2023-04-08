// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SquirrelManiaGameInstance.generated.h"

UCLASS()
class SQUIRRELMANIA_API USquirrelManiaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "Active Players")
		int m_NrPlayers = 0;

	void SetNrActivePlayers(int controllerIndex);
	int GetActivePlayersNr() const { return m_NrPlayers; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Victory")
	int m_WinningPlayerIndex = -1;
	void SetWinningPlayerIndex(int index);
	int GetWinningPlayerIndex() const { return m_WinningPlayerIndex; }
};
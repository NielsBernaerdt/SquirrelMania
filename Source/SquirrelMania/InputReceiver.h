// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputReceiver.generated.h"

class ASquirrelCharacter;

UCLASS()
class SQUIRRELMANIA_API AInputReceiver : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "PlayerSpawning")
		TSubclassOf<ASquirrelCharacter> m_SquirrelCharacterBP;

	UFUNCTION()
		void SetPlayerIndex(int index) {m_CurrentPlayerIndex = index;}

	void ActivatePawn();
private:
	int m_CurrentPlayerIndex;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
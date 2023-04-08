// DAE - Group 15 - SquirrelMania
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SquirrelCharacter.h"
#include "SquirrelController.generated.h"

class ASquirrelManiaGameModeBase;

UCLASS()
class SQUIRRELMANIA_API ASquirrelController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION()
		void ResetPawnReference();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	static bool m_IsFirstInputGiven;

	UPROPERTY(BlueprintReadOnly)
		FVector m_PunchDirection = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
		bool m_bIsInMenu = true;

	UPROPERTY(BlueprintReadWrite)
		float m_bChargeMinTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "Vibration")
		UForceFeedbackEffect* m_pChargeVibration;

	float m_ChargeAmount;
private:
	virtual void SetupInputComponent() override;

	void MoveRight(float value);
	void MoveUp(float value);
	void Jump();
	void StopJumping();
	void Attack();
	void DropDown();
	void ResetFallingThrough();
	void ResetDropdownPressed();
	void RestartGame();
	void QuitGame();
	void HeavyAttackCharge();
	void HeavyAttackRelease();
	void IncreasePower();
	void SetIndicationVisible();
	void SetIndicationHidden();

	ASquirrelCharacter* m_pPawn;
	ASquirrelManiaGameModeBase* m_pGameMode;
	bool m_bEnableDebugMessages = false; //Prevent from spamming messages BEFORE player is activated.
	int m_DroppedDownPressedAmount;

	FTimerHandle m_DelayTimer;
	FTimerHandle m_RetriggerableDelayTimer;
	FTimerHandle m_ChargeTimer;

};
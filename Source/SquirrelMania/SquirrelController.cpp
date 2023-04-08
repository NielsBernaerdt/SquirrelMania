// DAE - Group 15 - SquirrelMania
#include "SquirrelController.h"
#include "SquirrelManiaGameModeBase.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

bool ASquirrelController::m_IsFirstInputGiven = true;

void ASquirrelController::BeginPlay()
{
	m_IsFirstInputGiven = false;

	AGameModeBase* pGamemode = GetWorld()->GetAuthGameMode();
	if (pGamemode)
	{
		m_pGameMode = Cast<ASquirrelManiaGameModeBase>(pGamemode);
		if(!m_pGameMode &&
			GEngine)
		{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Output: %s"), *FString{"SQUIRRELCONTROLLER BEGINPLAY - GAMEMODE CAST FAILED"}));
		}
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Output: %s"), *FString{ "SQUIRRELCONTROLLER BEGINPLAY - GAMEMODE NOT FOUND" }));
	}
}

void ASquirrelController::Tick(float DeltaSeconds)
{
	if(m_pGameMode)
	{
		if(m_pGameMode->m_bIsGameActive == true)
		{
			EnableInput(this);
		}
		else
		{
			DisableInput(this);
		}
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Output: %s"), *FString{"SQUIRRELCONTROLLER TICK - GAMEMODE NOT FOUND"}));
	}

	if (m_pPawn && m_pPawn->m_bCharging == true)
	{
		m_ChargeAmount += GetWorld()->GetDeltaSeconds();
	}
		
}

void ASquirrelController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Connect the action and axis mappings from the project settinsg to these functions - UE4
	InputComponent->BindAxis("MoveRight", this, &ASquirrelController::MoveRight);
	InputComponent->BindAxis("MoveUp", this, &ASquirrelController::MoveUp);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ASquirrelController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ASquirrelController::StopJumping);
	InputComponent->BindAction("Attack", IE_Released, this, &ASquirrelController::Attack);
	InputComponent->BindAction("DropDown", IE_Pressed, this, &ASquirrelController::DropDown);
	InputComponent->BindAction("Restart", IE_Pressed, this, &ASquirrelController::RestartGame);
	InputComponent->BindAction("Quit", IE_Pressed, this, &ASquirrelController::QuitGame);
	InputComponent->BindAction("HeavyAttack", IE_Pressed, this, &ASquirrelController::HeavyAttackCharge);
	InputComponent->BindAction("HeavyAttack", IE_Released, this, &ASquirrelController::HeavyAttackRelease);
	InputComponent->BindAction("HighlightPlayer", IE_Pressed, this, &ASquirrelController::SetIndicationVisible);
	InputComponent->BindAction("HighlightPlayer", IE_Released, this, &ASquirrelController::SetIndicationHidden);
}
void ASquirrelController::MoveRight(float value)
{
	if (m_bIsInMenu)
		return;

	if (m_pPawn
		&& m_pPawn->m_bStunned == true)
		return;

	if(m_pPawn && m_pPawn->m_bCharging == true)
	{
		return;
	}

	if (m_pPawn && m_pPawn->m_bHeavyAttack == true)
	{
		return;
	}

	float deadZone = 0.1f;
	if(value > deadZone || value < -deadZone)
		m_IsFirstInputGiven = true;

	if (m_pPawn)
	{
		m_pPawn->AddMovementInput({ 0, -value, 0 });

		//Instantly set the rotation towards movement direction
		if (-value > 0)
		{
			m_pPawn->SetActorRotation(FVector{ 0,1,0 }.Rotation());

			m_PunchDirection.X = -1.f;
		}
		else if (-value < 0)
		{
			m_pPawn->SetActorRotation(FVector{ 0,-1,0 }.Rotation());

			m_PunchDirection.X = 1.f;
		}
	}
	else if (m_bEnableDebugMessages == true
		&& GEngine)
	{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SQUIRRELCONTROLLER MOVERIGHT - PAWN NULLPTR"));
	}
}
void ASquirrelController::MoveUp(float value)
{
	if (m_bIsInMenu)
		return;

	if (m_pPawn	&& m_pPawn->m_bStunned == true)
		return;

	m_PunchDirection.Y = value;
	float deadZone = 0.1f;
	if (value > deadZone || value < -deadZone)
	{
		m_PunchDirection.X = 0.f;
	}
}
void ASquirrelController::Jump()
{
	if (m_bIsInMenu)
		return;

	if (m_pPawn
		&& m_pPawn->m_bStunned == true)
		return;

	m_IsFirstInputGiven = true;

	if (m_pPawn)
	{
		m_pPawn->Jump();
	}
	else if (m_bEnableDebugMessages == true
		&& GEngine)
	{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SQUIRRELCONTROLLER JUMP - PAWN NULLPTR"));
	}
}
void ASquirrelController::StopJumping()
{
	if (m_pPawn)
	{
		m_pPawn->StopJumping();
	}
	else if (m_bEnableDebugMessages == true
		&& GEngine)
	{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SQUIRRELCONTROLLER STOPJUMPING - PAWN NULLPTR"));
	}
}
void ASquirrelController::Attack()
{
	if (m_bIsInMenu)
		return;

	if (m_pPawn
		&& m_pPawn->m_bStunned == true)
		return;

	m_IsFirstInputGiven = true;

	if (m_pPawn)
	{
		m_pPawn->m_bAttack = true;
	}
	else if (m_bEnableDebugMessages == true
		&& GEngine)
	{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SQUIRRELCONTROLLER ATTACK - PAWN NULLPTR"));
	}
}

void ASquirrelController::DropDown()
{
	if (m_bIsInMenu)
		return;

	if (m_pPawn
		&& m_pPawn->m_bStunned == true)
		return;

	m_IsFirstInputGiven = true;

	if (m_pPawn == nullptr)
		return;

	//if (m_pPawn->JumpCurrentCount != 0)
	//{
	//	m_pPawn->m_bGroundPound = true;
	//	return;
	//}

	m_DroppedDownPressedAmount++;
	m_DroppedDownPressedAmount = FMath::Clamp(m_DroppedDownPressedAmount, 0, 2);

	if(m_DroppedDownPressedAmount == 2)
	{
		m_pPawn->m_FallingThrough = true;
		GetWorld()->GetTimerManager().SetTimer(m_DelayTimer, this, &ASquirrelController::ResetFallingThrough, 0.4f, false);
	}

	//Retriggerable delay
	GetWorld()->GetTimerManager().ClearTimer(m_RetriggerableDelayTimer);
	GetWorld()->GetTimerManager().SetTimer(m_RetriggerableDelayTimer, this, &ASquirrelController::ResetDropdownPressed, 0.5f, false);
}

void ASquirrelController::ResetFallingThrough()
{
	m_pPawn->m_FallingThrough = false;
}

void ASquirrelController::ResetDropdownPressed()
{
	m_DroppedDownPressedAmount = 0;
}

void ASquirrelController::ResetPawnReference()
{
	m_pPawn = Cast<ASquirrelCharacter>(GetPawn());
	m_bEnableDebugMessages = true;
}

void ASquirrelController::RestartGame()
{
	m_IsFirstInputGiven = false;
	UGameplayStatics::OpenLevel(GetWorld(), "SceneMainMenu");
}

void ASquirrelController::QuitGame()
{
	FGenericPlatformMisc::RequestExit(true);
}

void ASquirrelController::HeavyAttackCharge()
{
	if (m_bIsInMenu)
		return;

	if (m_pPawn	&& m_pPawn->m_bStunned == true)
		return;

	if (m_pPawn)
	{
		m_ChargeAmount = 0;
		m_pPawn->m_KnockbackRangeHeavy = m_pPawn->m_KnockbackRangeHeavyDefault;
		m_pPawn->m_bCharging = true;
		IncreasePower();

		//VIBRATION
		if (m_pChargeVibration)
		{
			FForceFeedbackParameters p;
			p.Tag = FName("ChargeVibration");

			ClientPlayForceFeedback(m_pChargeVibration,p);
			
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple,
				                                 FString::Printf(TEXT("Output: %s"), *FString{"No ChargeVibration found"}));
			}
		}
		
	}
	else if (m_bEnableDebugMessages == true
		&& GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SQUIRRELCONTROLLER CHARGING - PAWN NULLPTR"));
	}
}

void ASquirrelController::HeavyAttackRelease()
{
	if (m_bIsInMenu)
		return;

	/*if (m_pPawn
		&& m_pPawn->m_bStunned == true)
		return;*/

	if (m_pPawn
		&& m_pPawn->m_bStunned == true)
	{
		m_pPawn->DestroyChargeParticles();
		m_pPawn->m_bCharging = false;
		m_pPawn->m_bHeavyReset = true;
		if (m_pChargeVibration)
		{
			ClientStopForceFeedback(m_pChargeVibration, FName("ChargeVibration"));
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple,
					FString::Printf(TEXT("Output: %s"), *FString{ "No ChargeVibration found" }));
			}
		}
		return;
	}
		


	if (m_pPawn)
	{
		m_pPawn->DestroyChargeParticles();
		m_pPawn->m_bCharging = false;
		//Check if its charging long enough
		if(m_ChargeAmount > m_bChargeMinTime)
		{
			m_pPawn->m_bHeavyAttack = true;
			
		} else
		{
			m_pPawn->m_bHeavyReset = true;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Not Long enough"));
		}
		//VIBRATION STOP
		if (m_pChargeVibration)
		{
			ClientStopForceFeedback(m_pChargeVibration,FName("ChargeVibration"));
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple,
					FString::Printf(TEXT("Output: %s"), *FString{ "No ChargeVibration found" }));
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Release"));
	}
	else if (m_bEnableDebugMessages == true
		&& GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SQUIRRELCONTROLLER HEAVY ATTACK - PAWN NULLPTR"));
	}
}

void ASquirrelController::IncreasePower()
{
	if (m_pPawn
		&& m_pPawn->m_bStunned == true)
		return;

	if (m_pPawn)
	{
		if(m_pPawn->m_bCharging)
		{
			m_pPawn->m_KnockbackRangeHeavy += 100.f;
			GetWorld()->GetTimerManager().SetTimer(m_ChargeTimer, this, &ASquirrelController::IncreasePower, 0.5f, false);
		}
	}
	else if (m_bEnableDebugMessages == true
		&& GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SQUIRRELCONTROLLER CHARGING - PAWN NULLPTR"));
	}
}

void ASquirrelController::SetIndicationVisible()
{
	if(m_pPawn)
		m_pPawn->SetVisiblityIndication(true);
}

void ASquirrelController::SetIndicationHidden()
{
	if(m_pPawn)
		m_pPawn->SetVisiblityIndication(false);
}
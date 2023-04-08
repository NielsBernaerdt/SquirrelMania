// DAE - Group 15 - SquirrelMania
#include "InputReceiver.h"
#include "ParallaxCamera.h"
#include "SquirrelCharacter.h"
#include "SquirrelController.h"
#include "SquirrelManiaGameInstance.h"
#include "SquirrelManiaGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AInputReceiver::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	InputComponent->BindAction("ActivatePlayer", IE_Released, this, &AInputReceiver::ActivatePawn);
}
void AInputReceiver::ActivatePawn()
{
	if(UGameplayStatics::GetPlayerController(GetWorld(), m_CurrentPlayerIndex))
	{
		ASquirrelCharacter* pPlayerPawn = GetWorld()->SpawnActor<ASquirrelCharacter>(m_SquirrelCharacterBP, GetActorTransform());
		pPlayerPawn->SetCorrectMesh(m_CurrentPlayerIndex + 1);

		if (pPlayerPawn)
		{
			//Spawn & Possess Player Character
			const auto pController = UGameplayStatics::GetPlayerController(GetWorld(), m_CurrentPlayerIndex);
			pController->GetPawn()->Destroy();
			pController->Possess(pPlayerPawn);
			Cast<ASquirrelController>(pController)->ResetPawnReference();

			const auto pGamemode = Cast<ASquirrelManiaGameModeBase>(GetWorld()->GetAuthGameMode());
			if( pGamemode )
			{
				pGamemode->SetPlayerReferences();
			}
			else if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,FString::Printf(TEXT("Output: %s"), *FString{"INPUTRECEIVER ACTIVATEPAWN - CAMERA NULLPTR"}));
			}
			Cast<USquirrelManiaGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetNrActivePlayers(m_CurrentPlayerIndex);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("INPUTRECEIVER ACTIVATEPAWN - PAWN NULLPTR"));
		}
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SQUIRRELCONTROLLER MOVERIGHT - PLAYERCONTROLLER NULLPTR"));
	}
}
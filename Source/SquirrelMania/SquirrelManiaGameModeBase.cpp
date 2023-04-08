// DAE - Group 15 - SquirrelMania
#include "SquirrelManiaGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "InputReceiver.h"
#include "SquirrelController.h"
#include "SquirrelManiaGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/CharacterMovementComponent.h"
//LevelSequencer
#include <LevelSequenceActor.h>
#include <LevelSequencePlayer.h>
#include <MovieSceneSequencePlayer.h>

ASquirrelManiaGameModeBase::ASquirrelManiaGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FClassFinder<AInputReceiver> ItemClass(TEXT("/Game/PlayerCharacter/Blueprints/BP_InputReceiver"));

	if (ItemClass.Class != nullptr)
	{
		m_InputReceiverRef = ItemClass.Class;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SQUIRRELMANIAGAMEMODEBASE CONSTRUCTOR - PATH TO BLUEPRINT INCORRECT"));
	}
}

void ASquirrelManiaGameModeBase::BeginPlay()
{
	TArray<AActor*> pPlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), pPlayerStarts);

	int nrActivePlayers = Cast<USquirrelManiaGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->GetActivePlayersNr();
	int index{};
	for (auto pPlayerStart : pPlayerStarts)
	{
		for (const auto pCorrectPlayerStart : pPlayerStarts)
		{
			const APlayerStart* pPlayerSpawn = Cast<APlayerStart>(pCorrectPlayerStart);
			if (FCString::Atoi(*pPlayerSpawn->PlayerStartTag.ToString()) == index)
			{
				UGameplayStatics::CreatePlayer(GetWorld(), -1, true);
				AInputReceiver* pCurrentPlayer = GetWorld()->SpawnActor<AInputReceiver>(m_InputReceiverRef, pPlayerStarts[index]->GetActorTransform());
				UGameplayStatics::GetPlayerController(GetWorld(), index)->Possess(pCurrentPlayer);

				if (pCurrentPlayer)
				{
					pCurrentPlayer->SetPlayerIndex(index);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("SQUIRRELMANIAGAMEMODEBASE BEGINPLAY - NO PAWN FOUND"));
				}
				
				if(nrActivePlayers !=  0)
				{
					--nrActivePlayers;
					pCurrentPlayer->ActivatePawn();
				}
			}
		}
		++index;
	}
}

void ASquirrelManiaGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (m_bStartGameOnce == false)
	{
		const auto pSquirrelController = Cast<ASquirrelController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (pSquirrelController
			&& pSquirrelController->m_IsFirstInputGiven == true)
		{
			m_bStartGameOnce = true;

			ALevelSequenceActor* pLevelSequenceActor = Cast<ALevelSequenceActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALevelSequenceActor::StaticClass()));
			if (pLevelSequenceActor)
				pLevelSequenceActor->SequencePlayer->Play();
			else if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
					FString::Printf(TEXT("Output: %s"), *FString{ "SQUIRRELMANIAGAMEMODEBASE TICK - NO LEVELSEQUENCER FOUND" }));
			}
		}
	}
}

void ASquirrelManiaGameModeBase::StartGame()
{
	m_bIsGameActive = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Output: %s"), *FString{"I was hiding"}));
	}
}
void ASquirrelManiaGameModeBase::StopGame()
{
	m_bIsGameActive = false;
}

void ASquirrelManiaGameModeBase::CheckVictoryCondition()
{
	int activePlayers = m_pPlayers.Num();
	int currentIndex = 0;
	int winnerIndex = 0;
	for (const auto pPlayer : m_pPlayers)
	{
		ASquirrelCharacter* pCharacter = Cast<ASquirrelCharacter>(pPlayer);
		if (pCharacter->m_Lives < 1)
		{
			auto newLocation = pPlayer->GetActorLocation();
			newLocation.X -= 1000;
			pPlayer->SetActorLocation(newLocation);
			--activePlayers;
		}
		else
		{
			winnerIndex = currentIndex;
		}
		++currentIndex;
	}

	if (activePlayers == 1)
	{
		Cast<USquirrelManiaGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SetWinningPlayerIndex(winnerIndex);
		UGameplayStatics::OpenLevel(GetWorld(), "SceneVictoryMenu");
	}
}

void ASquirrelManiaGameModeBase::SetPlayerReferences()
{
	m_pPlayers.Empty();

	TArray<AActor*> pPlayerArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASquirrelCharacter::StaticClass(), pPlayerArray);

	for (auto pPlayer : pPlayerArray)
	{
		auto pPawn = Cast<ASquirrelCharacter>(pPlayer);

		if (!pPawn)
			break;

		m_pPlayers.AddUnique(pPawn);
	}
}
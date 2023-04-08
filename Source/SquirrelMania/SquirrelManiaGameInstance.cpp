// DAE - Group 15 - SquirrelMania
#include "SquirrelManiaGameInstance.h"

void USquirrelManiaGameInstance::SetNrActivePlayers(int controllerIndex)
{
	if (controllerIndex + 1 > m_NrPlayers)
	{
		m_NrPlayers = controllerIndex + 1;
	}
}

void USquirrelManiaGameInstance::SetWinningPlayerIndex(int index)
{
	m_WinningPlayerIndex = index;
}
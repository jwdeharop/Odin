#include "OdinGameModeBase.h"

void AOdinGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	//AOD_BasePlayerState* PlayerState = NewPlayer ? NewPlayer->GetPlayerState<AOD_BasePlayerState>() : nullptr;
	//if (PlayerState)
	//{
	//	PlayerState->Initialize();
	//}
	Super::PostLogin(NewPlayer);
}

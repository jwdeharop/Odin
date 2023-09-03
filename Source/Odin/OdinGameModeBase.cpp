// Copyright Epic Games, Inc. All Rights Reserved.


#include "OdinGameModeBase.h"

#include "Actors/OD_BasePlayerState.h"
#include "Libraries/OD_NetLibrary.h"

void AOdinGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	if (UOD_NetLibrary::IsServer(this))
	{
		FString s;
	}
	AOD_BasePlayerState* PlayerState = NewPlayer ? NewPlayer->GetPlayerState<AOD_BasePlayerState>() : nullptr;
	if (PlayerState)
	{
		PlayerState->Initialize();
	}
	Super::PostLogin(NewPlayer);
}

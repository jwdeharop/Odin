#include "Controllers/OD_BasePlayerController.h"

#include "Actors/OD_BasePlayerState.h"

void AOD_BasePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnControllerGetsPlayerState.ExecuteIfBound(GetPlayerState<AOD_BasePlayerState>());
}

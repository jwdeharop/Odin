#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"

void AOD_ElementalPlayerController::BeginPlay()
{
	Super::BeginPlay();
	OnRep_PlayerState();
}

void AOD_ElementalPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnClientGetsPlayerState.Broadcast(GetPlayerState<APlayerState>());	
}
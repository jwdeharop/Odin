#include "OD_ElementalPlayerState.h"
#include "Libraries/OD_NetLibrary.h"
#include "UnrealNetwork.h"

void AOD_ElementalPlayerState::Server_SetCurrentDamageType_Implementation(EOD_ElementalDamageType DamageType)
{
	// If both slots are basic, we can change the current slot.
	if (CurrentPlayerStats.CurrentDamageType == EOD_ElementalDamageType::Basic && CurrentPlayerStats.SecondSlot == EOD_ElementalDamageType::Basic)
	{
		CurrentPlayerStats.CurrentDamageType = DamageType;
		return;
	}

	if (CurrentPlayerStats.SecondSlot == EOD_ElementalDamageType::Basic)
	{
		CurrentPlayerStats.SecondSlot = DamageType;
		return;
	}

	CurrentPlayerStats.CurrentDamageType = DamageType;
}

EOD_ElementalDamageType AOD_ElementalPlayerState::GetCurrentDamageType() const
{
	return CurrentPlayerStats.CurrentDamageType;
}

void AOD_ElementalPlayerState::TakeDamage(float Damage)
{
	CurrentPlayerStats.CurrentHealth -= Damage;
	OnClientStatsChanged.ExecuteIfBound(CurrentPlayerStats);
}

void AOD_ElementalPlayerState::OnRep_PlayerStats() const
{
	OnClientStatsChanged.ExecuteIfBound(CurrentPlayerStats);
}

void AOD_ElementalPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (UOD_NetLibrary::IsServer(this))
	{
		CurrentPlayerStats.MaxHealth = 100.f;
		CurrentPlayerStats.CurrentHealth = CurrentPlayerStats.MaxHealth;
		CurrentPlayerStats.CurrentDamageType = EOD_ElementalDamageType::Basic;
		CurrentPlayerStats.SecondSlot = EOD_ElementalDamageType::Basic;
	}
}

void AOD_ElementalPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOD_ElementalPlayerState, CurrentPlayerStats);
}
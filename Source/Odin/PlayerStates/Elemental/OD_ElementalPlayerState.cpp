#include "OD_ElementalPlayerState.h"
#include "Libraries/OD_NetLibrary.h"
#include "UnrealNetwork.h"

void AOD_ElementalPlayerState::Server_SetCurrentDamageType_Implementation(EOD_ElementalDamageType DamageType)
{
	// If both slots are basic, we can change the current slot.
	if (CurrentPlayerStats.CurrentDamageType == EOD_ElementalDamageType::Basic && CurrentPlayerStats.SecondSlot == EOD_ElementalDamageType::Basic)
	{
		CurrentPlayerStats.CurrentDamageType = DamageType;
		OnRep_PlayerStats();
		return;
	}

	if (CurrentPlayerStats.SecondSlot == EOD_ElementalDamageType::Basic)
	{
		CurrentPlayerStats.SecondSlot = DamageType;
		OnRep_PlayerStats();
		return;
	}

	if (DamageType == CurrentPlayerStats.SecondSlot && DamageType != CurrentPlayerStats.CurrentDamageType)
	{
		Server_ChangePrimaryDamageType();
		return;
	}

	CurrentPlayerStats.CurrentDamageType = DamageType;
	OnRep_PlayerStats();
}

void AOD_ElementalPlayerState::Server_ChangePrimaryDamageType_Implementation()
{
	if (CurrentPlayerStats.CurrentDamageType == CurrentPlayerStats.SecondSlot)
		return;

	const EOD_ElementalDamageType AuxDamage = CurrentPlayerStats.SecondSlot;
	CurrentPlayerStats.SecondSlot = CurrentPlayerStats.CurrentDamageType;
	CurrentPlayerStats.CurrentDamageType = AuxDamage;

	OnRep_PlayerStats();
}

EOD_ElementalDamageType AOD_ElementalPlayerState::GetCurrentDamageType() const
{
	return CurrentPlayerStats.CurrentDamageType;
}

void AOD_ElementalPlayerState::LocalTakeDamage(float Damage)
{
	CurrentPlayerStats.CurrentHealth -= Damage;
	OnRep_PlayerStats();
	OnClientStatsChanged.Broadcast(CurrentPlayerStats);
}

void AOD_ElementalPlayerState::OnRep_PlayerStats() const
{
	OnClientStatsChanged.Broadcast(CurrentPlayerStats);
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

		// Just for listener servers.
		OnRep_PlayerStats();
	}
}

void AOD_ElementalPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOD_ElementalPlayerState, CurrentPlayerStats);
}
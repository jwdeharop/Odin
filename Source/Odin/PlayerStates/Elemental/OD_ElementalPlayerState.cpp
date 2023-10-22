#include "OD_ElementalPlayerState.h"
#include "Libraries/OD_NetLibrary.h"
#include "UnrealNetwork.h"

void AOD_ElementalPlayerState::Server_SetCurrentDamageType_Implementation(EOD_ElementalDamageType DamageType)
{
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
		CurrentPlayerStats.CurrentDamageType = EOD_ElementalDamageType::Synthetic;
		CurrentPlayerStats.SecondSlot = EOD_ElementalDamageType::Synthetic;

		// Just for listener servers.
		OnRep_PlayerStats();
	}
}

void AOD_ElementalPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AOD_ElementalPlayerState, CurrentPlayerStats);
}
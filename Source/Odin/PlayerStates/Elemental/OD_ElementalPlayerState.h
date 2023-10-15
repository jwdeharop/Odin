#pragma once

#include "CoreMinimal.h"
#include "Odin.h"
#include "GameFramework/PlayerState.h"
#include "OD_ElementalPlayerState.generated.h"

USTRUCT(BlueprintType)
struct FOD_PlayerStats
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
		float MaxHealth = 0.f;
	UPROPERTY(BlueprintReadOnly)
		float CurrentHealth = 0.f;
	UPROPERTY(BlueprintReadOnly)
		EOD_ElementalDamageType CurrentDamageType = EOD_ElementalDamageType::Basic;
	UPROPERTY(BlueprintReadOnly)
		EOD_ElementalDamageType SecondSlot = EOD_ElementalDamageType::Basic; 
};

DECLARE_DELEGATE_OneParam(FOnStatsChanged, FOD_PlayerStats);

UCLASS(config=Game)
class AOD_ElementalPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
		void Server_SetCurrentDamageType(EOD_ElementalDamageType DamageType);

	FOnStatsChanged OnClientStatsChanged;

	EOD_ElementalDamageType GetCurrentDamageType() const;
	void TakeDamage(float Damage);

private:
	UPROPERTY(ReplicatedUsing=OnRep_PlayerStats)
		FOD_PlayerStats CurrentPlayerStats;

	UFUNCTION()
		void OnRep_PlayerStats() const;

protected:
	virtual void BeginPlay() override;

};


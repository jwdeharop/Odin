#pragma once

#include "CoreMinimal.h"
#include "Odin.h"
#include "GameFramework/PlayerState.h"
#include "OD_ElementalPlayerState.generated.h"

USTRUCT(BlueprintType)
struct FOD_PlayerStats
{
	GENERATED_BODY()

	UPROPERTY()
		float MaxHealth = 0.f;
	UPROPERTY()
		float CurrentHealth = 0.f;
	UPROPERTY()
		EOD_ElementalDamageType CurrentDamageType = EOD_ElementalDamageType::Basic;
};

UCLASS(config=Game)
class AOD_ElementalPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
		void Server_SetCurrentDamageType(EOD_ElementalDamageType DamageType);

	EOD_ElementalDamageType GetCurrentDamageType() const;

private:
	UPROPERTY(ReplicatedUsing=OnRep_PlayerStats)
		FOD_PlayerStats CurrentPlayerStats;

	UFUNCTION()
		void OnRep_PlayerStats();

protected:
	virtual void BeginPlay() override;

};


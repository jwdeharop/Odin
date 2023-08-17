#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EOD_SelfDistinction : uint8
{
	None = 0,
	Player,
	NPC
};

UENUM(BlueprintType)
enum class EOD_PickUpType : uint8
{
	None = 0,
	LWeapon, // Swords, etc etc
	RWeapon, // bows, etc etc
	Shield,
	Consumable,

	Num_Types
};


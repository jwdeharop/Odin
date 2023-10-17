#pragma once

#include "CoreMinimal.h"

ODIN_API DECLARE_LOG_CATEGORY_EXTERN(DataAssetLog, Log, All);

UENUM(BlueprintType)
enum class EOD_InventoryType : uint8
{
	None = 0,
	RWeapon, // Swords
	LWeapon, // Bows
	Shield,
	Consumable,

	Num_Types
};

UENUM(BlueprintType)
enum class EOD_ElementalDamageType : uint8
{
	Basic,
	Rock,
	Wood,
	Water,
	Thunder,
	Wind,
	Fire
};




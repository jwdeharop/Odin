#include "OD_DamageTypesDataAsset.h"

bool FOD_DamageTypeInfo::IsNemesis(EOD_ElementalDamageType OtherDamageType) const
{
	return Nemesis.Find(OtherDamageType) != INDEX_NONE;
}

bool FOD_DamageTypeInfo::IsResistance(EOD_ElementalDamageType OtherDamageType) const
{
	return Resistance.Find(OtherDamageType) != INDEX_NONE;
}

bool FOD_DamageTypeInfo::IsPrey(EOD_ElementalDamageType OtherDamageType) const
{
	return Prey.Find(OtherDamageType) != INDEX_NONE;
}

bool FOD_DamageTypeInfo::IsFull(EOD_ElementalDamageType OtherDamageType) const
{
	return Full.Find(OtherDamageType) != INDEX_NONE;
}

float UOD_DamageTypesDataAsset::GetDamageMultiplier(EOD_ElementalDamageType ReceiverDamage, EOD_ElementalDamageType InstigatorDamage) const
{
	const FOD_DamageTypeInfo* DamageType = DamageTypes.FindByPredicate([InstigatorDamage](const FOD_DamageTypeInfo& DamageInfo)
	{
		return DamageInfo.DamageType == InstigatorDamage;
	});

	if (!DamageType)
		return FullMultiplier;

	// If we have information of the damage, we need to know it's multiplier.
	if (DamageType->IsNemesis(ReceiverDamage))
	{
		return NemesisMultiplier;
	}
	if (DamageType->IsPrey(ReceiverDamage))
	{
		return PreyMultiplier;
	}
	if (DamageType->IsResistance(ReceiverDamage))
	{
		return ResistanceMultiplier;
	}

	return FullMultiplier;
}

#include "Components/Elemental/OD_CompDamage.h"
#include "DataAssets/Elemental/OD_DamageTypesDataAsset.h"

float UOD_CompDamage::GetBaseDamage() const
{
	return DamageTypeDataAsset ? DamageTypeDataAsset->BaseDamage : BaseDamage;
}

float UOD_CompDamage::GetDamage(EOD_ElementalDamageType OwnerDamage, EOD_ElementalDamageType OtherDamage) const
{
	if (!DamageTypeDataAsset)
		return BaseDamage;

	const float Multiplier = DamageTypeDataAsset->GetDamageMultiplier(OwnerDamage, OtherDamage);
	return GetBaseDamage() * Multiplier;
}

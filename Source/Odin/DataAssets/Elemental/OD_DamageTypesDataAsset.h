#pragma once

#include "CoreMinimal.h"
#include "Odin.h"
#include "OD_DamageTypesDataAsset.generated.h"

USTRUCT()
struct FOD_DamageTypeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		EOD_ElementalDamageType DamageType = EOD_ElementalDamageType::Synthetic;
	UPROPERTY(EditDefaultsOnly)
		TArray<EOD_ElementalDamageType> Nemesis;
	UPROPERTY(EditDefaultsOnly)
		TArray<EOD_ElementalDamageType> Resistance;
	UPROPERTY(EditDefaultsOnly)
		TArray<EOD_ElementalDamageType> Prey;
	UPROPERTY(EditDefaultsOnly)
		TArray<EOD_ElementalDamageType> Full;

	bool IsNemesis(EOD_ElementalDamageType OtherDamageType) const;
	bool IsResistance(EOD_ElementalDamageType OtherDamageType) const;
	bool IsPrey(EOD_ElementalDamageType OtherDamageType) const;
	bool IsFull(EOD_ElementalDamageType OtherDamageType) const;
	
};
UCLASS()
class ODIN_API UOD_DamageTypesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		TArray<FOD_DamageTypeInfo> DamageTypes;
	UPROPERTY(EditDefaultsOnly)
		float BaseDamage = 10.f;
	UPROPERTY(EditDefaultsOnly)
		float NemesisMultiplier = 0.25f;
	UPROPERTY(EditDefaultsOnly)
		float PreyMultiplier = 2.f;
	UPROPERTY(EditDefaultsOnly)
		float FullMultiplier = 1.f;
	UPROPERTY(EditDefaultsOnly)
		float ResistanceMultiplier = 0.5f;

	float GetDamageMultiplier(EOD_ElementalDamageType ReceiverDamage, EOD_ElementalDamageType InstigatorDamage) const;
};

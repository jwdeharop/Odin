#pragma once

#include "CoreMinimal.h"
#include "Odin.h"
#include "Components/ActorComponent.h"
#include "OD_CompDamage.generated.h"

class UOD_DamageTypesDataAsset;

UCLASS()
class ODIN_API UOD_CompDamage : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		float GetBaseDamage() const;
	UFUNCTION(BlueprintPure)
		float GetDamage(EOD_ElementalDamageType OwnerDamage, EOD_ElementalDamageType OtherDamage) const;

private:
	UPROPERTY(EditDefaultsOnly)
		UOD_DamageTypesDataAsset* DamageTypeDataAsset = nullptr;
	UPROPERTY(EditDefaultsOnly)
		float BaseDamage = 10.f;

};

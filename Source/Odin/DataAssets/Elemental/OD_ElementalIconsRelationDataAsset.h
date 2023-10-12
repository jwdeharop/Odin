#pragma once

#include "Odin.h"
#include "OD_ElementalIconsRelationDataAsset.generated.h"

USTRUCT()
struct FOD_ElementIconsRelation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		EOD_ElementalDamageType ElementType = EOD_ElementalDamageType::Basic;
	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr<UTexture2D> Icon = nullptr;
};

UCLASS()
class ODIN_API UOD_ElementalIconsRelationDataAsset : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
		TArray<FOD_ElementIconsRelation> AllIcons;
};
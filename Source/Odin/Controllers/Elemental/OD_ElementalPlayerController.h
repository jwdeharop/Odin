#pragma once

#include "Odin.h"
#include "GameFramework/PlayerController.h"
#include "OD_ElementalPlayerController.generated.h"

class UOD_ElementalIconsRelationDataAsset;
struct FOD_ElementIconsRelation;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnControllerGetsPlayerState, APlayerState*);
UCLASS(config=Game)
class AOD_ElementalPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		UOD_ElementalIconsRelationDataAsset* ElementalIcons;

	FOnControllerGetsPlayerState OnClientGetsPlayerState;

	UFUNCTION(BlueprintPure)
		FLinearColor GetElementColor(EOD_ElementalDamageType ElementType) const;
	UFUNCTION(BlueprintPure)
		TSoftObjectPtr<UTexture2D> GetElementIcon(EOD_ElementalDamageType ElementType) const;

protected:
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;

private:
	const FOD_ElementIconsRelation* GetIconRelation(EOD_ElementalDamageType ElementType) const;
	
};


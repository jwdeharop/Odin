#include "Controllers/Elemental/OD_ElementalPlayerController.h"

#include "DataAssets/Elemental/OD_ElementalIconsRelationDataAsset.h"
#include "Libraries/OD_NetLibrary.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"

FLinearColor AOD_ElementalPlayerController::GetElementColor(EOD_ElementalDamageType ElementType) const
{
	const FOD_ElementIconsRelation* IconRelation = GetIconRelation(ElementType);
	return IconRelation ? IconRelation->ElementColor : FLinearColor::White;
}

TSoftObjectPtr<UTexture2D> AOD_ElementalPlayerController::GetElementIcon(EOD_ElementalDamageType ElementType) const
{
	const FOD_ElementIconsRelation* IconRelation = GetIconRelation(ElementType);
	return IconRelation ? IconRelation->Icon : nullptr;
}

void AOD_ElementalPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UOD_NetLibrary::IsDedicatedServer(this))
	{
		// Dedicated server does not need this data asset.
		ElementalIcons = nullptr;
		return;
	}

	OnRep_PlayerState();
}

void AOD_ElementalPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (ElementalIcons)
	{
		for (FOD_ElementIconsRelation Icon : ElementalIcons->AllIcons)
		{
			const UTexture2D* LoadedTexture = Icon.Icon.LoadSynchronous();
			if (!LoadedTexture)
			{
				UE_LOG(DataAssetLog, Error, TEXT("Couldn't load Icon %s correctly"), *Icon.Icon.GetAssetName());
				continue;
			}

			UE_LOG(DataAssetLog, Log, TEXT("Icon %s load correctly"), *Icon.Icon.GetAssetName());
		}
	}
	OnClientGetsPlayerState.Broadcast(GetPlayerState<APlayerState>());	
}

const FOD_ElementIconsRelation* AOD_ElementalPlayerController::GetIconRelation(EOD_ElementalDamageType ElementType) const
{
	const FOD_ElementIconsRelation* IconRelation = ElementalIcons ? ElementalIcons->AllIcons.FindByPredicate([ElementType](const FOD_ElementIconsRelation& IconsRelation)
	{
		return IconsRelation.ElementType == ElementType;
	}) : nullptr;

	return IconRelation;
}

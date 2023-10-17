#include "Libraries/OD_GUILibrary.h"
#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "DataAssets/Elemental/OD_ElementalIconsRelationDataAsset.h"
#include "Libraries/OD_BaseLibrary.h"

FLinearColor UOD_GUILibrary::GetElementColor(const UObject* WorldObjectContext, EOD_ElementalDamageType ElementType)
{
	const AOD_ElementalPlayerController* LocalPlayerController = UOD_BaseLibrary::GetLocalPlayerController(WorldObjectContext);
	const FOD_ElementIconsRelation* IconRelation = LocalPlayerController ? LocalPlayerController->GetIconRelation(ElementType) : nullptr;
	return IconRelation ? IconRelation->ElementColor : FLinearColor::White;
}

TSoftObjectPtr<UTexture2D> UOD_GUILibrary::GetElementIcon(const UObject* WorldObjectContext, EOD_ElementalDamageType ElementType)
{
	const AOD_ElementalPlayerController* LocalPlayerController = UOD_BaseLibrary::GetLocalPlayerController(WorldObjectContext);
	const FOD_ElementIconsRelation* IconRelation = LocalPlayerController ? LocalPlayerController->GetIconRelation(ElementType) : nullptr;
	return IconRelation ? IconRelation->Icon : nullptr;
}

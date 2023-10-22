#include "Widgets/Elemental/OD_WidgetRadialProgressBar.h"
#include "Image.h"
#include "Characters/Elemental/OD_ElementalCharacter.h"
#include "Components/OD_CompInteractable.h"
#include "Components/OD_CompInteraction.h"
#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "Interfaces/OD_InteractionInterface.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Libraries/OD_BaseLibrary.h"

namespace UOD_WdigetRadialProgressBar_Consts
{
	const FName PercentageValueStr = TEXT("Percentage");
}

void UOD_WdigetRadialProgressBar::SetPercentage(float Percentage) const
{
	if (!DynamicInstance || !ImageRound)
		return;
	
	ImageRound->SetBrushFromMaterial(DynamicInstance);
	DynamicInstance->SetScalarParameterValue(UOD_WdigetRadialProgressBar_Consts::PercentageValueStr, Percentage);
}

void UOD_WdigetRadialProgressBar::OnInteractionAvailable()
{
	IOD_InteractionInterface* InteractionActor = Cast<IOD_InteractionInterface>(CompInteractionPtr->GetCurrentInteractActor());
	UOD_CompInteractable* CompInteractable = InteractionActor ? InteractionActor->GetCompInteractable() : nullptr;
	if (CompInteractable && CompInteractable->IsHoldInteraction())
	{
		CompInteractable->OnInteractionHoldStartsClient.AddUObject(this,&UOD_WdigetRadialProgressBar::HoldInteractionStarts);
		CompInteractable->OnInteractionEndClient.AddUObject(this,&UOD_WdigetRadialProgressBar::HoldInteractionEnds);
	}
}

void UOD_WdigetRadialProgressBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (!MaterialInterface)
		return;

	DynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, MaterialInterface);
	SetPercentage(0.f);

	GatherCompInteractionPtr();
	if (CompInteractionPtr)
	{
		CompInteractionPtr->InteractionAvailable.AddUObject(this, &UOD_WdigetRadialProgressBar::OnInteractionAvailable);
		CompInteractionPtr->LostInteraction.AddUObject(this, &UOD_WdigetRadialProgressBar::OnInteractionLost);
	}
}

void UOD_WdigetRadialProgressBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bCanUpdateProgress || !CompInteractionPtr)
		return;

	CurrentProgressDuration += InDeltaTime;
	const float Percentage = CurrentProgressDuration / CompInteractionPtr->GetObjectHoldInteractionTime();
	if (Percentage <= 1.f)
	{
		SetPercentage(Percentage);
	}
}

void UOD_WdigetRadialProgressBar::HoldInteractionStarts()
{
	SetPercentage(0.f);
	CurrentProgressDuration = 0.f;
	GatherCompInteractionPtr();
	bCanUpdateProgress = true;
}

void UOD_WdigetRadialProgressBar::HoldInteractionEnds(bool bSucceed)
{
	CurrentProgressDuration = 0.f;
	bCanUpdateProgress = false;
	SetPercentage(0.f);
}

void UOD_WdigetRadialProgressBar::GatherCompInteractionPtr()
{
	AOD_ElementalCharacter* MyCharacter = UOD_BaseLibrary::GetLocalPlayerCharacter(this);
	CompInteractionPtr = MyCharacter ? MyCharacter->GetCompInteraction() : nullptr;
}

void UOD_WdigetRadialProgressBar::OnInteractionLost(AActor* Actor)
{
	IOD_InteractionInterface* InteractionActor = Cast<IOD_InteractionInterface>(Actor);
    UOD_CompInteractable* CompInteractable = InteractionActor ? InteractionActor->GetCompInteractable() : nullptr;
    if (CompInteractable && CompInteractable->IsHoldInteraction())
    {
    	CompInteractable->OnInteractionStartsClient.RemoveAll(this);
    	CompInteractable->OnInteractionEndClient.RemoveAll(this);
    }
}

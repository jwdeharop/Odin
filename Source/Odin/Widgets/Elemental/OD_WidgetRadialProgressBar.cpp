#include "Widgets/Elemental/OD_WidgetRadialProgressBar.h"
#include "Image.h"
#include "Characters/Elemental/OD_ElementalCharacter.h"
#include "Components/OD_CompInteraction.h"
#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "Kismet/KismetMaterialLibrary.h"

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
		CompInteractionPtr->HoldInteractionStarts.BindUObject(this, &UOD_WdigetRadialProgressBar::HoldInteractionStarts);
		CompInteractionPtr->HoldInteractionEnds.BindUObject(this, &UOD_WdigetRadialProgressBar::HoldInteractionEnds);
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

void UOD_WdigetRadialProgressBar::HoldInteractionEnds()
{
	CurrentProgressDuration = 0.f;
	bCanUpdateProgress = false;
	SetPercentage(0.f);
}

void UOD_WdigetRadialProgressBar::GatherCompInteractionPtr()
{
	const AOD_ElementalPlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController<AOD_ElementalPlayerController>();
	AOD_ElementalCharacter* MyCharacter = MyPlayerController ? MyPlayerController->GetPawn<AOD_ElementalCharacter>() : nullptr;
	CompInteractionPtr = MyCharacter ? MyCharacter->GetCompInteraction() : nullptr;
}

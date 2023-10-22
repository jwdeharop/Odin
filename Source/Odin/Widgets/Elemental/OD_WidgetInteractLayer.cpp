#include "Widgets/Elemental/OD_WidgetInteractLayer.h"

#include "Characters/Elemental/OD_ElementalCharacter.h"
#include "Components/OD_CompInteractable.h"
#include "Components/OD_CompInteraction.h"
#include "Interfaces/OD_InteractionInterface.h"
#include "Libraries/OD_BaseLibrary.h"

void UOD_WidgetInteractLayer::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	AOD_ElementalCharacter* LocalCharacter = UOD_BaseLibrary::GetLocalPlayerCharacter(this);

	if (UOD_CompInteraction* CompInteraction = LocalCharacter ? LocalCharacter->GetCompInteraction() : nullptr)
	{
		CompInteraction->InteractionAvailable.AddUObject(this, &UOD_WidgetInteractLayer::OnInteractionAvailable);
		CompInteraction->LostInteraction.AddUObject(this, &UOD_WidgetInteractLayer::OnInteractionLost);
	}
}

void UOD_WidgetInteractLayer::OnInteractionLost(AActor* Actor) const
{
	IOD_InteractionInterface* InteractionActor = Cast<IOD_InteractionInterface>(Actor);
	UOD_CompInteractable* CompInteractable = InteractionActor ? InteractionActor->GetCompInteractable() : nullptr;
	if (CompInteractable && CompInteractable->IsHoldInteraction())
	{
		CompInteractable->OnInteractionStartsClient.RemoveAll(this);
		CompInteractable->OnInteractionEndClient.RemoveAll(this);
		CompInteractable->OnInteractionAvailabilityChange.RemoveAll(this);
	}
}

void UOD_WidgetInteractLayer::OnInteractionAvailabilityChange(bool bCanInteract)
{
	const float Opacity = bCanInteract ? 1.f : 0.5;
	SetRenderOpacity(Opacity);
}

void UOD_WidgetInteractLayer::OnInteractionAvailable()
{
	AOD_ElementalCharacter* LocalCharacter = UOD_BaseLibrary::GetLocalPlayerCharacter(this);

	if (const UOD_CompInteraction* CompInteraction = LocalCharacter ? LocalCharacter->GetCompInteraction() : nullptr)
	{
		IOD_InteractionInterface* InteractionActor = Cast<IOD_InteractionInterface>(CompInteraction->GetCurrentInteractActor());
		UOD_CompInteractable* CompInteractable = InteractionActor ? InteractionActor->GetCompInteractable() : nullptr;
		if (!CompInteractable)
			return;

		if (!CompInteractable->OnInteractionAvailabilityChange.IsBoundToObject(this))
		{
			CompInteractable->OnInteractionAvailabilityChange.AddUObject(this, &UOD_WidgetInteractLayer::OnInteractionAvailabilityChange);
		}
		OnInteractionAvailabilityChange(CompInteractable->CanInteract());
	}
}

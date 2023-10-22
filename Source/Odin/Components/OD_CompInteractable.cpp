#include "Components/OD_CompInteractable.h"

void UOD_CompInteractable::ClientInteractionStart()
{
	if (InteractionData.bCanInteract)
	{
		if (GetHoldInteractTime() > 0.f)
		{
			OnInteractionHoldStartsClient.Broadcast();
			GetOwner()->GetWorldTimerManager().SetTimer(HoldTimer, this, &UOD_CompInteractable::HoldInteractionEnds, GetHoldInteractTime());
		}
		else
		{
			OnInteractionStartsClient.Broadcast();
		}
	}
}

void UOD_CompInteractable::ServerInteractionStart()
{
	OnInteractionStartServer.Broadcast();
}

void UOD_CompInteractable::ServerInteractionEnds(bool bSucceed)
{
	OnInteractionEndServer.Broadcast(bSucceed);
}

void UOD_CompInteractable::ClientInteractionEnds(bool bSucceed)
{
	OnInteractionEndClient.Broadcast(bSucceed);
}

void UOD_CompInteractable::SetCanInteract(bool bCanInteract)
{
	InteractionData.bCanInteract = bCanInteract;
	OnInteractionAvailabilityChange.Broadcast(bCanInteract);
}

bool UOD_CompInteractable::IsHoldInteraction() const
{
	return InteractionData.HoldInteraction > 0.f;
}

bool UOD_CompInteractable::CanInteract() const
{
	return InteractionData.bCanInteract;
}

const FUF_InteractionData& UOD_CompInteractable::GetInteractionData() const
{
	return InteractionData;
}

float UOD_CompInteractable::GetHoldInteractTime() const
{
	return InteractionData.HoldInteraction;
}

void UOD_CompInteractable::HoldInteractionEnds()
{
	ClientInteractionEnds(true);
}

#include "Actors/Elemental/OD_ElementalExtractionPoint.h"

#include "Characters/Elemental/OD_ElementalCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"

AOD_ElementalExtractionPoint::AOD_ElementalExtractionPoint()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void AOD_ElementalExtractionPoint::Client_InteractionSuccessful_Implementation()
{
	OnInteractionSucess.ExecuteIfBound();
}

void AOD_ElementalExtractionPoint::StartInteraction(ACharacter* InteractionInstigator)
{
	MyInstigator = InteractionInstigator;
	GetWorldTimerManager().SetTimer(InteractionTimer, this, &AOD_ElementalExtractionPoint::InteractionSuccessful, GetHoldInteractTime());
}

void AOD_ElementalExtractionPoint::CancelInteraction()
{
	if (InteractionTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(InteractionTimer);
	}
}

void AOD_ElementalExtractionPoint::InteractionSuccessful()
{
	if (InteractionTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(InteractionTimer);
	}

	const AOD_ElementalCharacter* MyCharacter = Cast<AOD_ElementalCharacter>(MyInstigator);
	if (AOD_ElementalPlayerState* MyPlayerState = MyCharacter ? MyCharacter->GetPlayerState<AOD_ElementalPlayerState>() : nullptr)
	{
		MyPlayerState->Server_SetCurrentDamageType(DamageType);
	}

	CancelInteraction();
	Client_InteractionSuccessful();
}

float AOD_ElementalExtractionPoint::GetHoldInteractTime()
{
	return InteractionTime;
}
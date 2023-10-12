#include "Actors/Elemental/OD_ElementalExtractionPoint.h"

#include "Characters/Elemental/OD_ElementalCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"

AOD_ElementalExtractionPoint::AOD_ElementalExtractionPoint()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void AOD_ElementalExtractionPoint::Server_StartInteraction_Implementation(ACharacter* InteractionInstigator)
{
	MyInstigator = InteractionInstigator;
	GetWorldTimerManager().SetTimer(InteractionTimer, this, &AOD_ElementalExtractionPoint::InteractionSuccessful, 3.f);
}

void AOD_ElementalExtractionPoint::Server_StopInteraction_Implementation()
{
	if (InteractionTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(InteractionTimer);
	}
}

void AOD_ElementalExtractionPoint::StartInteraction(ACharacter* InteractionInstigator)
{
	Server_StartInteraction(InteractionInstigator);
}

void AOD_ElementalExtractionPoint::CancelInteraction()
{
	Server_StopInteraction();
}

bool AOD_ElementalExtractionPoint::IsHoldInteraction()
{
	return true;
}

void AOD_ElementalExtractionPoint::InteractionSuccessful()
{
	if (InteractionTimer.IsValid())
	{
		GetWorldTimerManager().ClearTimer(InteractionTimer);
	}

	AOD_ElementalCharacter* MyCharacter = Cast<AOD_ElementalCharacter>(MyInstigator);
	AOD_ElementalPlayerState* MyPlayerState = MyCharacter ? MyCharacter->GetPlayerState<AOD_ElementalPlayerState>() : nullptr;
	if (MyPlayerState)
	{
		MyPlayerState->Server_SetCurrentDamageType(DamageType);
	}
}

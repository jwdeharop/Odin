#include "Actors/Elemental/OD_ElementalExtractionPoint.h"
#include "Characters/Elemental/OD_ElementalCharacter.h"
#include "Components/OD_CompInteractable.h"
#include "Components/StaticMeshComponent.h"
#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "Libraries/OD_BaseLibrary.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"

AOD_ElementalExtractionPoint::AOD_ElementalExtractionPoint()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	CompInteractable = CreateDefaultSubobject<UOD_CompInteractable>(TEXT("CompInteractable"));
}

UOD_CompInteractable* AOD_ElementalExtractionPoint::GetCompInteractable()
{
	return CompInteractable;
}

void AOD_ElementalExtractionPoint::StartInteraction(ACharacter* InteractionInstigator)
{
	// Start interaction starts on client.
	MyInstigator = InteractionInstigator;
	GetWorldTimerManager().SetTimer(InteractionTimer, this, &AOD_ElementalExtractionPoint::InteractionSuccessful, CompInteractable->GetHoldInteractTime());
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

	const AOD_ElementalCharacter* MyCharacter = UOD_BaseLibrary::GetLocalPlayerCharacter(this);
	if (AOD_ElementalPlayerState* MyPlayerState = MyCharacter ? MyCharacter->GetPlayerState<AOD_ElementalPlayerState>() : nullptr)
	{
		MyPlayerState->Server_SetCurrentDamageType(DamageType);
	}

	CancelInteraction();
}

void AOD_ElementalExtractionPoint::PrepareInteraction(bool bCanInteract)
{
	if (AOD_ElementalCharacter* LocalCharacter = UOD_BaseLibrary::GetLocalPlayerCharacter(this))
	{
		LocalCharacter->InteractionDamageType = bCanInteract ? DamageType : EOD_ElementalDamageType::Synthetic;
	}	
}

void AOD_ElementalExtractionPoint::BeginPlay()
{
	Super::BeginPlay();

	const AOD_ElementalCharacter* LocalCharacter = UOD_BaseLibrary::GetLocalPlayerCharacter(this);
	if (LocalCharacter && LocalCharacter->IsLocallyControlled())
	{
		CompInteractable->OnInteractionEndClient.AddUObject(this, &AOD_ElementalExtractionPoint::InteractionEndsClient);

		if (AOD_ElementalPlayerState* LocalPLayerState = UOD_BaseLibrary::GetLocalPlayerState(this))
		{
			OnClientGetsPlayerState(LocalPLayerState);
		}
		else if (AOD_ElementalPlayerController* LocalPlayerController = UOD_BaseLibrary::GetLocalPlayerController(this))
		{
			LocalPlayerController->OnClientGetsPlayerState.AddUObject(this, &AOD_ElementalExtractionPoint::OnClientGetsPlayerState);
		}
	}
}

void AOD_ElementalExtractionPoint::InteractionStartedOnServer(bool bSucceed)
{
}

void AOD_ElementalExtractionPoint::InteractionEndsClient(bool bSucceed)
{
	if (bSucceed)
	{
		InteractionSuccessful();
	}
	else
	{
		CancelInteraction();
	}
}

void AOD_ElementalExtractionPoint::OnClientsStatsChanged(FOD_PlayerStats PlayerStats)
{
	if (CompInteractable)
	{
		CompInteractable->SetCanInteract(PlayerStats.CurrentDamageType != DamageType);	
	}
}

void AOD_ElementalExtractionPoint::OnClientGetsPlayerState(APlayerState* PlayerState)
{
	if (AOD_ElementalPlayerState* ElementalPlayerState = Cast<AOD_ElementalPlayerState>(PlayerState))
	{
		if (!ElementalPlayerState->OnClientStatsChanged.IsBoundToObject(this))
		{
			ElementalPlayerState->OnClientStatsChanged.AddUObject(this, &AOD_ElementalExtractionPoint::OnClientsStatsChanged);
			OnClientsStatsChanged(ElementalPlayerState->GetPlayerStats());
		}
	}
}

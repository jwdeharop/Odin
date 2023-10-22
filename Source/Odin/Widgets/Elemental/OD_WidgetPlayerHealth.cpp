#include "Widgets/Elemental/OD_WidgetPlayerHealth.h"
#include "TextBlock.h"
#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "Libraries/OD_BaseLibrary.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"

void UOD_WidgetPlayerHealth::OnClientGetsPlayerState(APlayerState* PlayerState)
{
	if (AOD_ElementalPlayerState* LocalPlayerState = Cast<AOD_ElementalPlayerState>(PlayerState))
	{
		LocalPlayerState->OnClientStatsChanged.AddUObject(this, &UOD_WidgetPlayerHealth::OnClientStatsChanged);
		OnClientStatsChanged(LocalPlayerState->GetPlayerStats());
	}
}

void UOD_WidgetPlayerHealth::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	AOD_ElementalPlayerState* PlayerState = UOD_BaseLibrary::GetLocalPlayerState(this);
	if (!PlayerState)
	{
		if (AOD_ElementalPlayerController* PlayerController = UOD_BaseLibrary::GetLocalPlayerController(this))
		{
			PlayerController->OnClientGetsPlayerState.AddUObject(this, &UOD_WidgetPlayerHealth::OnClientGetsPlayerState);
		}
		return;
	}

	if (!PlayerState->OnClientStatsChanged.IsBoundToObject(this))
	{
		PlayerState->OnClientStatsChanged.AddUObject(this, &UOD_WidgetPlayerHealth::OnClientStatsChanged);
	}
}

void UOD_WidgetPlayerHealth::OnClientStatsChanged(FOD_PlayerStats PlayerStats)
{
	if (PlayerHealth)
	{
		PlayerHealth->SetText(FText::AsNumber(PlayerStats.CurrentHealth));
	}
}

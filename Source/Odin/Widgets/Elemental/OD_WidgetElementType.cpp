#include "Widgets/Elemental/OD_WidgetElementType.h"
#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "Libraries/OD_BaseLibrary.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"

void UOD_WidgetElementType::OnClientGetsPlayerState(APlayerState* PlayerState)
{
	if (AOD_ElementalPlayerState* ElementalPlayerState = Cast<AOD_ElementalPlayerState>(PlayerState))
	{
		if (!ElementalPlayerState->OnClientStatsChanged.IsBoundToObject(this))
		{
			ElementalPlayerState->OnClientStatsChanged.AddUObject(this, &UOD_WidgetElementType::OnClientStatsChanged);
		}
	}
}

void UOD_WidgetElementType::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AOD_ElementalPlayerState* PlayerState = UOD_BaseLibrary::GetLocalPlayerState(this);
	if (!PlayerState)
	{
		if (AOD_ElementalPlayerController* PlayerController = UOD_BaseLibrary::GetLocalPlayerController(this))
		{
			PlayerController->OnClientGetsPlayerState.AddUObject(this, &UOD_WidgetElementType::OnClientGetsPlayerState);
		}
		return;
	}

	if (!PlayerState->OnClientStatsChanged.IsBoundToObject(this))
	{
		PlayerState->OnClientStatsChanged.AddUObject(this, &UOD_WidgetElementType::OnClientStatsChanged);
	}
}

void UOD_WidgetElementType::OnClientStatsChanged(FOD_PlayerStats PlayerStats)
{
	BP_ChangeElementIcon(PlayerStats.CurrentDamageType);
}

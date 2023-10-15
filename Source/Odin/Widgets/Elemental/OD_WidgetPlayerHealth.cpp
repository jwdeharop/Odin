#include "Widgets/Elemental/OD_WidgetPlayerHealth.h"
#include "TextBlock.h"
#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"

void UOD_WidgetPlayerHealth::OnClientGetsPlayerState(APlayerState* PlayerState)
{
	if (AOD_ElementalPlayerState* LocalPlayerState = Cast<AOD_ElementalPlayerState>(PlayerState))
	{
		LocalPlayerState->OnClientStatsChanged.AddUObject(this, &UOD_WidgetPlayerHealth::OnClientStatsChanged);
	}
}

void UOD_WidgetPlayerHealth::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AOD_ElementalPlayerController* PlayerController = Cast<AOD_ElementalPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PlayerController)
		return;

	AOD_ElementalPlayerState* PlayerState = PlayerController->GetPlayerState<AOD_ElementalPlayerState>();
	if (!PlayerState)
	{
		PlayerController->OnClientGetsPlayerState.AddUObject(this, &UOD_WidgetPlayerHealth::OnClientGetsPlayerState);
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

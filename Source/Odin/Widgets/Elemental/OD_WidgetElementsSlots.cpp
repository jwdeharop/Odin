#include "Widgets/Elemental/OD_WidgetElementsSlots.h"

#include "OD_WidgetElementType.h"
#include "Controllers/Elemental/OD_ElementalPlayerController.h"
#include "Libraries/OD_BaseLibrary.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"

void UOD_WidgetElementsSlots::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AOD_ElementalPlayerState* PlayerState = UOD_BaseLibrary::GetLocalPlayerState(this);
	if (!PlayerState)
	{
		if (AOD_ElementalPlayerController* PlayerController = UOD_BaseLibrary::GetLocalPlayerController(this))
		{
			PlayerController->OnClientGetsPlayerState.AddUObject(this, &UOD_WidgetElementsSlots::OnClientGetsPlayerState);
		}
		return;
	}

	if (!PlayerState->OnClientStatsChanged.IsBoundToObject(this))
	{
		PlayerState->OnClientStatsChanged.AddUObject(this, &UOD_WidgetElementsSlots::OnClientStatsChanged);
	}
}

void UOD_WidgetElementsSlots::OnClientGetsPlayerState(APlayerState* PlayerState)
{
	if (AOD_ElementalPlayerState* ElementalPlayerState = Cast<AOD_ElementalPlayerState>(PlayerState))
	{
		if (!ElementalPlayerState->OnClientStatsChanged.IsBoundToObject(this))
		{
			ElementalPlayerState->OnClientStatsChanged.AddUObject(this, &UOD_WidgetElementsSlots::OnClientStatsChanged);
		}
	}
}

void UOD_WidgetElementsSlots::OnClientStatsChanged(FOD_PlayerStats PlayerStats)
{
	if (!FirstSlot && !SecondSlot)
		return;
	
	FirstSlot->BP_ChangeElementIcon(PlayerStats.CurrentDamageType);
	SecondSlot->BP_ChangeElementIcon(PlayerStats.SecondSlot);
	const bool bSecondSlotNotVisible = PlayerStats.CurrentDamageType == EOD_ElementalDamageType::Synthetic && PlayerStats.SecondSlot == EOD_ElementalDamageType::Synthetic; 
	SecondSlot->SetVisibility(bSecondSlotNotVisible ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
}

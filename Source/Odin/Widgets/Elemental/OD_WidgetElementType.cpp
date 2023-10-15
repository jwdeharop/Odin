#include "Widgets/Elemental/OD_WidgetElementType.h"

#include "Characters/Elemental/OD_ElementalCharacter.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"

void UOD_WidgetElementType::OnRepPlayerState(APlayerState* PlayerState)
{
	if (AOD_ElementalPlayerState* LocalPlayerState = Cast<AOD_ElementalPlayerState>(PlayerState))
	{
		LocalPlayerState->OnClientStatsChanged.BindUObject(this, &UOD_WidgetElementType::OnClientStatsChanged);
	}
}

void UOD_WidgetElementType::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	AOD_ElementalCharacter* PlayerCharacter = PlayerController ? PlayerController->GetPawn<AOD_ElementalCharacter>() : nullptr;
	if (!PlayerCharacter)
		return;

	PlayerCharacter->OnRepPlayerState.AddUObject(this, &UOD_WidgetElementType::OnRepPlayerState);
}

void UOD_WidgetElementType::OnClientStatsChanged(FOD_PlayerStats PlayerStats)
{
	BP_ChangeElementIcon(PlayerStats.CurrentDamageType);
}

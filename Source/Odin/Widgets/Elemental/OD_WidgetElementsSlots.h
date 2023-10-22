#pragma once

#include "Odin.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"
#include "OD_WidgetElementsSlots.generated.h"

class UOD_WidgetElementType;

UCLASS(config=Game)
class UOD_WidgetElementsSlots : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		UOD_WidgetElementType* FirstSlot = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		UOD_WidgetElementType* SecondSlot = nullptr;

	virtual void NativeOnInitialized() override;

private:
	void OnClientGetsPlayerState(APlayerState* PlayerState);
	void OnClientStatsChanged(FOD_PlayerStats PlayerStats);
};
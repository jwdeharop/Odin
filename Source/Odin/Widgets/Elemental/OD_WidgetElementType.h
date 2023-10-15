#pragma once

#include "Odin.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"
#include "OD_WidgetElementType.generated.h"

UCLASS(config=Game)
class UOD_WidgetElementType : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void BP_ChangeElementIcon(EOD_ElementalDamageType ElementType);

	void OnRepPlayerState(APlayerState* PlayerState);
	virtual void NativeOnInitialized() override;

private:
	void OnClientStatsChanged(FOD_PlayerStats PlayerStats);
};
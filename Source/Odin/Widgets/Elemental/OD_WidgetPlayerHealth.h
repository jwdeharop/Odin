#pragma once

#include "Odin.h"
#include "Blueprint/UserWidget.h"
#include "OD_WidgetPlayerHealth.generated.h"

class UTextBlock;
struct FOD_PlayerStats;
UCLASS(config=Game)
class UOD_WidgetPlayerHealth : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* PlayerHealth = nullptr;

	void OnClientGetsPlayerState(APlayerState* PlayerState);
	virtual void NativeOnInitialized() override;

private:
	void OnClientStatsChanged(FOD_PlayerStats PlayerStats);

};
#pragma once

#include "Odin.h"
#include "Blueprint/UserWidget.h"
#include "OD_WidgetInteractLayer.generated.h"

UCLASS(config=Game)
class UOD_WidgetInteractLayer : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	void OnInteractionLost(AActor* Actor) const;
	void OnInteractionAvailabilityChange(bool bCanInteract);
	void OnInteractionAvailable();
};
#pragma once

#include "Odin.h"
#include "Blueprint/UserWidget.h"
#include "OD_WidgetElementType.generated.h"

UCLASS(config=Game)
class UOD_WidgetElementType : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		void BP_ChangeElementIcon(EOD_ElementalDamageType ElementType);
};
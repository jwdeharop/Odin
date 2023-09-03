#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OD_UIInventorySlot.generated.h"

UCLASS()
class ODIN_API UOD_UIInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	int32 Index = -1;
	UFUNCTION(BlueprintImplementableEvent)
		void BP_CreateSlot(const FOD_InventoryValue& InventoryValue);

	void CreateSlot(const FOD_InventoryValue& InventoryValue, int32 NewIndex);
};

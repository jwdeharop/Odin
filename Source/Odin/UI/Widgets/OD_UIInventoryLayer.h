#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OD_UIInventoryLayer.generated.h"

class UOD_UIInventoryGrid;

UCLASS()
class ODIN_API UOD_UIInventoryLayer : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		UOD_UIInventoryGrid* InventoryGrid = nullptr;
};

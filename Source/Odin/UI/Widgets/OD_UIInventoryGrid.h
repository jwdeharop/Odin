#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/OD_CompInventory.h"
#include "OD_UIInventoryGrid.generated.h"

class UWrapBox;
class UScrollBox;
class UOD_UIInventorySlot;
class AOD_BasePlayerState;

UCLASS()
class ODIN_API UOD_UIInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UOD_UIInventorySlot> InventorySlot = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		UWrapBox* InventoryGrid = nullptr;

	void OnItemReceived(const FOD_InventoryValue& InventoryValue);
	void OnControllerGetsPlayerState(AOD_BasePlayerState* PlayerState);
	void OnInventoryTypeChanged(EOD_InventoryType InventoryType);

	void BindToEvents(AOD_BasePlayerState* PlayerState);
	void InitGrid(EOD_InventoryType InventoryType);
};

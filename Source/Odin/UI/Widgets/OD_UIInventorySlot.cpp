#include "UI/Widgets/OD_UIInventorySlot.h"
#include "Components/OD_CompInventory.h"

void UOD_UIInventorySlot::CreateSlot(const FOD_InventoryValue& InventoryValue, int32 NewIndex)
{
	Index = -1;
	if (!InventoryValue.ItemName.IsNone())
	{
		Index = NewIndex;
	}

	BP_CreateSlot(InventoryValue);
}

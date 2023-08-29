#include "Components/OD_CompInventory.h"

#include "Actors/OD_PickUpBase.h"
#include "Libraries/OD_NetLibrary.h"

void UOD_CompInventory::Server_AddItemToInventory_Implementation(EOD_InventoryType InventoryType, const FOD_InventoryValue& InventoryValue, AOD_PickUpBase* PickUp)
{
	// We want only the server to add items to inventory. This call is redundant, but better check than cry.
	if (!UOD_NetLibrary::IsServer(this))
		return;

	CurrentItemAdded.Reset();

	FOD_InventoryType* TypeInventory = Inventory.FindByPredicate([InventoryType](const FOD_InventoryType& TypeInventory)
	{
		return TypeInventory.Type == InventoryType;
	});

	if (TypeInventory)
	{
		AddItemToInventory(TypeInventory, InventoryValue);
		if (CurrentItemAdded.IsSet())
		{
			CurrentItemAdded.Reset();
			Client_NotifyItemAdded(InventoryValue);
			OnItemReceived_Server.Broadcast(InventoryValue);
		}

		// If item comes from a pick up, we will destroy it.
		if (PickUp)
		{
			PickUp->Destroy();
		}
	}
}

void UOD_CompInventory::InitInventories()
{
	for (uint8 Index = 0; Index < static_cast<uint8>(EOD_InventoryType::Num_Types); ++Index)
	{
		const EOD_InventoryType Type = static_cast<EOD_InventoryType>(Index);
		if (Type == EOD_InventoryType::None)
			continue;

		FOD_InventoryType InventoryType;
		InventoryType.Type = Type;

		Inventory.Add(InventoryType);
	}
}

void UOD_CompInventory::Client_NotifyItemAdded_Implementation(const FOD_InventoryValue& InventoryValue)
{
	OnItemReceived_Client.Broadcast(InventoryValue);
}

void UOD_CompInventory::AddItemToInventory(FOD_InventoryType* InventoryType, const FOD_InventoryValue& InventoryValue)
{
	if (!InventoryType)
		return;

	TArray<FOD_InventoryValue>& Items = InventoryType->Items;
	FOD_InventoryValue* ItemInInventory = Items.FindByPredicate([InventoryValue](const FOD_InventoryValue& ItemInInventory)
	{
		return InventoryValue.ItemName == ItemInInventory.ItemName;
	});

	// if item is already in inventory, add the stack if the object is stackable.
	if (ItemInInventory && ItemInInventory->bCanStack)
	{
		ItemInInventory->Quantity += InventoryValue.Quantity;
		CurrentItemAdded = *ItemInInventory;
		return;
	}

	FOD_InventoryValue Item;
	Item.Quantity = InventoryValue.Quantity;
	Item.ItemName = InventoryValue.ItemName;

	// If item does not exist or it is not stackable, add to the items.
	Items.Add(Item);

	CurrentItemAdded = Item;
}

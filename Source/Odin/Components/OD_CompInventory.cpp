#include "Components/OD_CompInventory.h"

#include "UnrealNetwork.h"
#include "Actors/OD_PickUpBase.h"
#include "Libraries/OD_NetLibrary.h"

void UOD_CompInventory::Server_AddItemToInventory_Implementation(const FOD_InventoryValue& InventoryValue, AOD_PickUpBase* PickUp)
{
	// We want only the server to add items to inventory. This call is redundant, but better check than cry.
	if (!UOD_NetLibrary::IsServer(this))
		return;

	CurrentItemAdded.Reset();

	FOD_InventoryType* TypeInventory = Inventory.FindByPredicate([InventoryValue](const FOD_InventoryType& TypeInventory)
	{
		return TypeInventory.Type == InventoryValue.Type;
	});

	if (TypeInventory)
	{
		AddItemToInventory(TypeInventory, InventoryValue);
		if (CurrentItemAdded.IsSet())
		{
			CurrentItemAdded.Reset();
			Client_NotifyItemAdded(InventoryValue);
			OnItemReceived_Server.Broadcast(InventoryValue);

			if (PickUp)
			{
				PickUp->Destroy();
			}
		}
	}
}

void UOD_CompInventory::Server_ChangeInventoryType_Implementation(bool bIncrease)
{
	constexpr uint8 MaxInventoryTypeIndex = static_cast<uint8>(EOD_InventoryType::Num_Types);
	uint8 CurrentInventoryTypeIndex = static_cast<uint8>(CurrentInventoryType);

	if (bIncrease)
	{
		++CurrentInventoryTypeIndex;
		if (MaxInventoryTypeIndex == CurrentInventoryTypeIndex)
		{
			CurrentInventoryTypeIndex = 1;
		}
	}
	else
	{
		--CurrentInventoryTypeIndex;
		if (CurrentInventoryTypeIndex == 0)
		{
			CurrentInventoryTypeIndex = MaxInventoryTypeIndex - 1;
		}
	}

	CurrentInventoryType = static_cast<EOD_InventoryType>(CurrentInventoryTypeIndex);
	Client_NotifyInventoryTypeChanged(CurrentInventoryType);
}

void UOD_CompInventory::Initialize()
{
	InitInventories();
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

	CurrentInventoryType = EOD_InventoryType::RWeapon;
	const FOD_ServerInventoryData InventoryData(Inventory, CurrentInventoryType);
	Client_CopyInventory(InventoryData);
}

void UOD_CompInventory::SetCurrentInventoryType(EOD_InventoryType InventoryType)
{
	CurrentInventoryType = InventoryType;
}

int32 UOD_CompInventory::GetCurrentInventoryTypeMaxItemsInInventory() const
{
	const FOD_InventoryType* InventoryType = Inventory.FindByPredicate([this](const FOD_InventoryType& InventoryType)
	{
		return CurrentInventoryType == InventoryType.Type;
	});

	return InventoryType ? InventoryType->MaxItemsInType : 0;
}

TArray<FOD_InventoryValue> UOD_CompInventory::GetItems(EOD_InventoryType InventoryType) const
{
	const FOD_InventoryType* MyInventoryType = Inventory.FindByPredicate([InventoryType](const FOD_InventoryType& TypeInInventory)
	{
		return InventoryType == TypeInInventory.Type;
	});

	if (!MyInventoryType)
		return TArray<FOD_InventoryValue>();

	return MyInventoryType->Items;
}

void UOD_CompInventory::Client_NotifyItemAdded_Implementation(const FOD_InventoryValue& InventoryValue)
{
	OnItemReceived_Client.Broadcast(InventoryValue);
}

void UOD_CompInventory::Client_NotifyInventoryTypeChanged_Implementation(EOD_InventoryType ServerInventoryType)
{
    CurrentInventoryType = ServerInventoryType;
	OnTypeInventoryChanged_Client.Broadcast(CurrentInventoryType);
}

void UOD_CompInventory::Client_CopyInventory_Implementation(const FOD_ServerInventoryData ServerData)
{
	Inventory = ServerData.Inventory;
	CurrentInventoryType = ServerData.InventoryType;
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
	Item.Type = InventoryValue.Type;

	// If item does not exist or it is not stackable, add to the items.
	Items.Add(Item);

	CurrentItemAdded = Item;
	const FOD_ServerInventoryData InventoryData(Inventory, CurrentInventoryType);
	Client_CopyInventory(InventoryData);
}

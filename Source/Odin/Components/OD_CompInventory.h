#pragma once

#include "CoreMinimal.h"
#include "Odin.h"
#include "Components/ActorComponent.h"
#include "OD_CompInventory.generated.h"

USTRUCT(BlueprintType)
struct FOD_InventoryValue
{
	GENERATED_BODY()

	// Mark with UPROPERTY so we don't lose information in RPC's.
	UPROPERTY(Transient)
		FName ItemName = NAME_None;
	UPROPERTY(Transient)
		int32 Quantity = 0;

	FText ItemDescription;
	bool bCanStack = true;
};

USTRUCT(BlueprintType)
struct FOD_InventoryType
{
	GENERATED_BODY()

	UPROPERTY(Transient)
		TArray<FOD_InventoryValue> Items;

	EOD_InventoryType Type = EOD_InventoryType::None;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemReceived, const FOD_InventoryValue&)

class AOD_PickUpBase;

UCLASS()
class ODIN_API UOD_CompInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		const TArray<FOD_InventoryType>& GetInventory() const { return Inventory; }

	UFUNCTION(Server, Reliable)
		void Server_AddItemToInventory(EOD_InventoryType InventoryType, const FOD_InventoryValue& InventoryValue, AOD_PickUpBase* PickUp = nullptr);

	FOnItemReceived OnItemReceived_Client;
	FOnItemReceived OnItemReceived_Server;

	void InitInventories();

private:
	TArray<FOD_InventoryType> Inventory;
	TOptional<FOD_InventoryValue> CurrentItemAdded;

	UFUNCTION(Client, Reliable)
		void Client_NotifyItemAdded(const FOD_InventoryValue& InventoryValue);

	void AddItemToInventory(FOD_InventoryType* InventoryType, const FOD_InventoryValue& InventoryValue);
};
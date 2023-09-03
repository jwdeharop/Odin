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
	UPROPERTY(Transient, BlueprintReadOnly)
		FName ItemName = NAME_None;
	UPROPERTY(Transient, BlueprintReadOnly)
		int32 Quantity = 0;
	UPROPERTY(Transient, BlueprintReadOnly)
		FText ItemDescription;
	UPROPERTY(Transient, BlueprintReadOnly)
		EOD_InventoryType Type = EOD_InventoryType::None;
	bool bCanStack = true;
};

USTRUCT(BlueprintType)
struct FOD_InventoryType
{
	GENERATED_BODY()

	UPROPERTY(Transient)
		TArray<FOD_InventoryValue> Items;

	UPROPERTY(Transient)
		EOD_InventoryType Type = EOD_InventoryType::None;

	UPROPERTY(Transient)
		int32 MaxItemsInType = 10;
};

USTRUCT()
struct FOD_ServerInventoryData
{
	GENERATED_BODY()

	FOD_ServerInventoryData() = default;
	FOD_ServerInventoryData(const TArray<FOD_InventoryType>& ServerInventory, EOD_InventoryType ServerType) : Inventory(ServerInventory), InventoryType(ServerType){};

	UPROPERTY(Transient)
		TArray<FOD_InventoryType> Inventory;
	UPROPERTY(Transient)
		EOD_InventoryType InventoryType = EOD_InventoryType::None;
	
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemReceived, const FOD_InventoryValue&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryTypeChanged, EOD_InventoryType);

class AOD_PickUpBase;

UCLASS()
class ODIN_API UOD_CompInventory : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
		const TArray<FOD_InventoryType>& GetInventory() const { return Inventory; }

	UFUNCTION(Server, Reliable)
		void Server_AddItemToInventory(const FOD_InventoryValue& InventoryValue, AOD_PickUpBase* PickUp = nullptr);
	UFUNCTION(Server, Reliable)
		void Server_ChangeInventoryType(bool bIncrease);

	FOnItemReceived OnItemReceived_Client;
	FOnItemReceived OnItemReceived_Server;
	FOnInventoryTypeChanged OnTypeInventoryChanged_Client;

	void Initialize();
	void InitInventories();
	void SetCurrentInventoryType(EOD_InventoryType InventoryType);
	EOD_InventoryType GetCurrentInventoryType() const { return CurrentInventoryType; }
	int32 GetCurrentInventoryTypeMaxItemsInInventory() const;
	TArray<FOD_InventoryValue> GetItems(EOD_InventoryType InventoryType) const;
private:
	UPROPERTY(Transient)
		EOD_InventoryType CurrentInventoryType = EOD_InventoryType::None;

	UPROPERTY()
		TArray<FOD_InventoryType> Inventory;

	TOptional<FOD_InventoryValue> CurrentItemAdded;

	UFUNCTION(Client, Reliable)
		void Client_NotifyItemAdded(const FOD_InventoryValue& InventoryValue);
	UFUNCTION(Client, Reliable)
		void Client_NotifyInventoryTypeChanged(EOD_InventoryType ServerInventoryType);
	UFUNCTION(Client, Reliable)
		void Client_CopyInventory(const FOD_ServerInventoryData ServerData);

	void AddItemToInventory(FOD_InventoryType* InventoryType, const FOD_InventoryValue& InventoryValue);
};
#include "UI/Widgets/OD_UIInventoryGrid.h"
#include "Actors/OD_BasePlayerState.h"
#include "Controllers/OD_BasePlayerController.h"
#include "UI/Widgets/OD_UIInventorySlot.h"
#include "WrapBox.h"

void UOD_UIInventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AOD_BasePlayerController* PlayerController = Cast<AOD_BasePlayerController>(GetOwningPlayer());
	if (!PlayerController)
		return;

	// Grid will bind to the inventory received/consumed.
	// Each time an item is received, we will create a InventorySlot to this item.
	AOD_BasePlayerState* PlayerState = PlayerController->GetPlayerState<AOD_BasePlayerState>();
	if (!PlayerState)
	{
		// If client does not have the PlayerState yet, we wait for it.
		PlayerController->OnControllerGetsPlayerState.BindUObject(this, &UOD_UIInventoryGrid::OnControllerGetsPlayerState);
		return;
	}

	BindToEvents(PlayerState);
	const UOD_CompInventory* InventoryComponent = PlayerState ? PlayerState->GetCompInventory() : nullptr;
	InitGrid(InventoryComponent ? InventoryComponent->GetCurrentInventoryTypeMaxItemsInInventory() : 0);
}

void UOD_UIInventoryGrid::OnItemReceived(const FOD_InventoryValue& InventoryValue)
{
	if (!InventorySlot.Get() || !InventoryGrid)
		return;

	const AOD_BasePlayerController* PlayerController = Cast<AOD_BasePlayerController>(GetOwningPlayer());
	const AOD_BasePlayerState* PlayerState = PlayerController ? PlayerController->GetPlayerState<AOD_BasePlayerState>() : nullptr;
	const UOD_CompInventory* InventoryComponent = PlayerState ? PlayerState->GetCompInventory() : nullptr;

	if (!InventoryComponent || InventoryComponent->GetCurrentInventoryType() != InventoryValue.Type)
		return;

	TArray<UWidget*> Result = InventoryGrid->GetAllChildren();
	UWidget** PanelSlot = Result.FindByPredicate([](UWidget* ContentToCheck)
	{
		const UOD_UIInventorySlot* InventorySlotToCheck = Cast<UOD_UIInventorySlot>(ContentToCheck);
		return InventorySlotToCheck && InventorySlotToCheck->Index == -1;
	});

	if (!PanelSlot)
	{
		UOD_UIInventorySlot* WidgetInventorySlot = Cast<UOD_UIInventorySlot>(CreateWidget(this, InventorySlot));
		if (!WidgetInventorySlot)
			return;

		WidgetInventorySlot->CreateSlot(InventoryValue, 1);
		InventoryGrid->AddChild(WidgetInventorySlot);
	}
	else
	{
		if (UOD_UIInventorySlot* InventorySlotToChange = Cast<UOD_UIInventorySlot>(*PanelSlot))
		{
			InventorySlotToChange->CreateSlot(InventoryValue, 1);
		}
	}
}

void UOD_UIInventoryGrid::OnControllerGetsPlayerState(AOD_BasePlayerState* PlayerState)
{
	BindToEvents(PlayerState);
	const UOD_CompInventory* InventoryComponent = PlayerState ? PlayerState->GetCompInventory() : nullptr;
	InitGrid(InventoryComponent ? InventoryComponent->GetCurrentInventoryTypeMaxItemsInInventory() : 0);
}

void UOD_UIInventoryGrid::OnInventoryTypeChanged(EOD_InventoryType InventoryType)
{
	const AOD_BasePlayerController* PlayerController = Cast<AOD_BasePlayerController>(GetOwningPlayer());
	const AOD_BasePlayerState* PlayerState = PlayerController ? PlayerController->GetPlayerState<AOD_BasePlayerState>() : nullptr;

	const UOD_CompInventory* InventoryComponent = PlayerState ? PlayerState->GetCompInventory() : nullptr;
	InitGrid(InventoryComponent ? InventoryComponent->GetCurrentInventoryTypeMaxItemsInInventory() : 0);
}

void UOD_UIInventoryGrid::BindToEvents(AOD_BasePlayerState* PlayerState)
{
	AOD_BasePlayerController* PlayerController = PlayerState ? Cast<AOD_BasePlayerController>(PlayerState->GetPlayerController()) : nullptr;
	if (!PlayerController)
		return;

	PlayerController->OnControllerGetsPlayerState.Unbind();

	if (UOD_CompInventory* InventoryComponent = PlayerState->GetCompInventory())
	{
		InventoryComponent->OnItemReceived_Client.AddUObject(this, &UOD_UIInventoryGrid::OnItemReceived);
		InventoryComponent->OnTypeInventoryChanged_Client.AddUObject(this, &UOD_UIInventoryGrid::OnInventoryTypeChanged);
	}
}

void UOD_UIInventoryGrid::InitGrid(int32 MaxItemsInGrid)
{
	if (!InventorySlot.Get() || !InventoryGrid)
		return;

	InventoryGrid->ClearChildren();

	const AOD_BasePlayerController* PlayerController = Cast<AOD_BasePlayerController>(GetOwningPlayer());
	const AOD_BasePlayerState* PlayerState = PlayerController ? PlayerController->GetPlayerState<AOD_BasePlayerState>() : nullptr;
	const UOD_CompInventory* CompInventory = PlayerState ? PlayerState->GetCompInventory() : nullptr;

	if (!CompInventory)
		return;

	for (int32 Index = 0; Index < MaxItemsInGrid; ++Index)
	{
		UOD_UIInventorySlot* WidgetInventorySlot = Cast<UOD_UIInventorySlot>(CreateWidget(this, InventorySlot));
		if (!WidgetInventorySlot)
			return;

		const TArray<FOD_InventoryValue> Items = CompInventory->GetItems(CompInventory->GetCurrentInventoryType());
		if (Items.IsEmpty() || Index > Items.Num() - 1)
		{
			WidgetInventorySlot->CreateSlot(FOD_InventoryValue(), -1);
		}
		else
		{
			FOD_InventoryValue InventoryValue = Items[Index];
			WidgetInventorySlot->CreateSlot(InventoryValue, 1);
		}

		InventoryGrid->AddChild(WidgetInventorySlot);
	}
}

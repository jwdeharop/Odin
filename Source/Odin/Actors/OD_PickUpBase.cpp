#include "Actors/OD_PickUpBase.h"

#include "OD_BaseItem.h"
#include "OD_CollisionChannels.h"
#include "Characters/OD_BaseCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/OD_CompInventory.h"
#include "Libraries/OD_NetLibrary.h"

AOD_PickUpBase::AOD_PickUpBase() : Super()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Collision"));

	MeshComponent->SetupAttachment(RootComponent);

	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetCollisionProfileName(OD_CollisionPresets::OD_Interact);

	SetReplicates(true);
}

void AOD_PickUpBase::StartInteraction()
{
	if (!BaseItem.Get())
		return;
	
	// The interaction of the pick up base will be to add the item to the inventory.
	// This add process will take place in the server.
	const APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	AOD_BaseCharacter* LocalCharacter = LocalPlayerController ? Cast<AOD_BaseCharacter>(LocalPlayerController->GetPawn()) : nullptr;
	if (UOD_CompInventory* CompInventory = LocalCharacter ? LocalCharacter->GetCompInventory() : nullptr)
	{
		FOD_InventoryValue Item;
		Item.ItemName = BaseItem.GetDefaultObject()->ItemName;
		Item.Quantity = Quantity;
		Item.Type = BaseItem.GetDefaultObject()->InventoryType;
		CompInventory->Server_AddItemToInventory(Item, this);
	}
}

void AOD_PickUpBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AOD_PickUpBase::PrepareInteraction(bool bCanInteract)
{
	// Change the interact widget visibility here.
}

void AOD_PickUpBase::SetOverlayMaterial(UMaterialInstance* NewOverlayMaterial)
{
	if (MeshComponent)
	{
		MeshComponent->SetOverlayMaterial(NewOverlayMaterial);
	}
}
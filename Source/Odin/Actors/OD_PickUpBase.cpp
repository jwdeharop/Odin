#include "Actors/OD_PickUpBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "OD_CollisionChannels.h"

AOD_PickUpBase::AOD_PickUpBase() : Super()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Collision"));

	MeshComponent->SetupAttachment(RootComponent);

	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetCollisionProfileName(OD_CollisionPresets::OD_Interact);
}

void AOD_PickUpBase::BeginPlay()
{
	Super::BeginPlay();
}

void AOD_PickUpBase::PrepareInteraction(bool bCanInteract)
{
	SetOverlayMaterial(bCanInteract ? OverlayMaterial : nullptr);
}

void AOD_PickUpBase::SetOverlayMaterial(UMaterialInstance* NewOverlayMaterial)
{
	if (MeshComponent)
	{
		MeshComponent->SetOverlayMaterial(NewOverlayMaterial);
	}
}

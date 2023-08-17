#include "Actors/OD_PickUpBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "OD_CollisionChannels.h"

AOD_PickUpBase::AOD_PickUpBase() : Super()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Interact Collision"));

	RootComponent = MeshComponent;

	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetCollisionProfileName(OD_CollisionPresets::OD_Interact);
}

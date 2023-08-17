#include "Components/OD_CompInteraction.h"

#include "OD_CollisionChannels.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

UOD_CompInteraction::UOD_CompInteraction() : Super()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOD_CompInteraction::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UOD_CompInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CanInteractWithAnyObject())
	{
		// Notify the object that is being interacted with.
	}
}

bool UOD_CompInteraction::CanInteractWithAnyObject() const
{
	FHitResult OutHit;
	if (!LineTraceSingle(OutHit))
		return false;

	return true;
}

bool UOD_CompInteraction::LineTraceSingle(FHitResult& OutHit) const
{
	const UWorld* World = GetWorld();
	if (!OwnerCharacter.IsValid() || !World)
		return false;

	FVector StartLocation = FVector::ZeroVector;
	FRotator CharacterRotation = FRotator::ZeroRotator;

	OwnerCharacter->GetActorEyesViewPoint(StartLocation, CharacterRotation);

	const FVector EndLocation = StartLocation + CharacterRotation.Vector() * InteractDistance;
	constexpr bool bTraceComplex = true;
	const FCollisionQueryParams CollisionParameters(NAME_None, bTraceComplex, OwnerCharacter.Get());
	return World->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, COLLISION_CHANNEL(InteractRaycast), CollisionParameters);
}

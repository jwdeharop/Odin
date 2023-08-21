#include "Components/OD_CompInteraction.h"

#include "OD_CollisionChannels.h"
#include "GameFramework/Character.h"
#include "Interfaces/OD_InteractionInterface.h"
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

	FHitResult OutHit;
	if (CanInteractWithAnyObject(OutHit))
	{
		CurrentInteractActor = OutHit.GetActor();
		if (IOD_InteractionInterface* InteractionInterface = Cast<IOD_InteractionInterface>(CurrentInteractActor.Get()))
		{
			// Notify the object that is being interacted with.
			constexpr bool bCanPrepareInteraction = true;
			InteractionInterface->PrepareInteraction(bCanPrepareInteraction);
			return;
		}
	}

	if (CurrentInteractActor.IsValid())
	{
		ResetInteraction(OutHit.GetActor());
	}
}

bool UOD_CompInteraction::CanInteractWithAnyObject(FHitResult& OutHit) const
{
	if (!LineTraceSingle(OutHit))
		return false;

	// We dont want to interact again with the same object.
	if (OutHit.GetActor() == CurrentInteractActor)
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

void UOD_CompInteraction::ResetInteraction(const AActor* ActorToReset)
{
	if (ActorToReset != CurrentInteractActor)
	{
		if (IOD_InteractionInterface* InteractionInterface = Cast<IOD_InteractionInterface>(CurrentInteractActor.Get()))
		{
			// Notify the object that the interaction has ended.
			constexpr bool bCanPrepareInteraction = false;
			InteractionInterface->PrepareInteraction(bCanPrepareInteraction);
		}
		CurrentInteractActor.Reset();
	}
}

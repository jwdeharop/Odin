#include "Components/OD_CompInteraction.h"

#include "OD_CollisionChannels.h"
#include "Camera/CameraComponent.h"
#include "Characters/Elemental/OD_ElementalCharacter.h"
#include "GameFramework/Character.h"
#include "Interfaces/OD_InteractionInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UOD_CompInteraction::UOD_CompInteraction(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOD_CompInteraction::StartInteraction()
{
	IOD_InteractionInterface* InteractionInterface = Cast<IOD_InteractionInterface>(CurrentInteractActor.Get());
	if (!InteractionInterface)
		return;

	InteractionInterface->StartInteraction(Cast<ACharacter>(GetOwner()));
}

void UOD_CompInteraction::StopInteraction()
{
	IOD_InteractionInterface* InteractionInterface = Cast<IOD_InteractionInterface>(CurrentInteractActor.Get());
	if (!InteractionInterface)
		return;

	InteractionInterface->CancelInteraction();
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
	EOD_InteractionType InteractionType = CanInteractWithAnyObject(OutHit);
	if (InteractionType == EOD_InteractionType::InteractionSuccess)
	{
		CurrentInteractActor = OutHit.GetActor();
		if (IOD_InteractionInterface* InteractionInterface = Cast<IOD_InteractionInterface>(CurrentInteractActor.Get()))
		{
			// Notify the object that is being interacted with.
			constexpr bool bCanPrepareInteraction = true;
			InteractionInterface->PrepareInteraction(bCanPrepareInteraction);
			InteractionAvailable.ExecuteIfBound();
			return;
		}

		InteractionType = EOD_InteractionType::InteractionLost;
	}

	if (CurrentInteractActor.Get() && (InteractionType == EOD_InteractionType::InteractionLost || InteractionType == EOD_InteractionType::NoInteraction))
	{
		ResetInteraction(OutHit.GetActor());
		LostInteraction.ExecuteIfBound();
	}
}

EOD_InteractionType UOD_CompInteraction::CanInteractWithAnyObject(FHitResult& OutHit) const
{
	if (!LineTraceSingle(OutHit))
		return EOD_InteractionType::NoInteraction;

	// We dont want to interact again with the same object.
	if (OutHit.GetActor() == CurrentInteractActor)
		return EOD_InteractionType::InteractionRepeated;

	return EOD_InteractionType::InteractionSuccess;
}

bool UOD_CompInteraction::LineTraceSingle(FHitResult& OutHit) const
{
	const UWorld* World = GetWorld();
	const AOD_ElementalCharacter* MyCharacter = Cast<AOD_ElementalCharacter>(OwnerCharacter);
	if (!MyCharacter || !World)
		return false;
	
	FVector StartLocation = FVector::ZeroVector;
	FRotator CharacterRotation = FRotator::ZeroRotator;

	OwnerCharacter->GetActorEyesViewPoint(StartLocation, CharacterRotation);
	const FVector EndLocation = StartLocation + CharacterRotation.Vector() * InteractDistance;
	constexpr bool bTraceComplex = true;
	const FCollisionQueryParams CollisionParameters(NAME_None, bTraceComplex, OwnerCharacter.Get());
	//UKismetSystemLibrary::DrawDebugLine(this, StartLocation, EndLocation, FLinearColor::Red, 100.f, 10.f);
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

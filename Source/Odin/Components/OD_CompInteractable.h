#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OD_CompInteractable.generated.h"

USTRUCT(BlueprintType)
struct FUF_InteractionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float HoldInteraction = 0.f;

	bool bCanInteract = true;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FInteractionEndServer, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FInteractionAvailabilityChanged, bool /*bCanInteract*/);

UCLASS()
class ODIN_API UOD_CompInteractable : public UActorComponent
{
	GENERATED_BODY()

public:
	FSimpleMulticastDelegate OnInteractionStartServer;
	FSimpleMulticastDelegate OnInteractionStartsClient;
	FSimpleMulticastDelegate OnInteractionHoldStartsClient;
	FInteractionEndServer OnInteractionEndServer;
	FInteractionEndServer OnInteractionEndClient;
	FInteractionAvailabilityChanged OnInteractionAvailabilityChange;

	void ClientInteractionStart();
	void ServerInteractionStart();
	void ServerInteractionEnds(bool bSucceed);
	void ClientInteractionEnds(bool bSucceed);
	void SetCanInteract(bool bCanInteract);
	
	bool IsHoldInteraction() const;
	bool CanInteract() const;
	const FUF_InteractionData& GetInteractionData() const;
	float GetHoldInteractTime() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FUF_InteractionData InteractionData;

private:
	FTimerHandle HoldTimer;

	void HoldInteractionEnds();
};

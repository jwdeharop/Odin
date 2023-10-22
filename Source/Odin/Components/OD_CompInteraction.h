#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OD_CompInteraction.generated.h"

class ACharacter;

enum class EOD_InteractionType : uint8
{
	InteractionLost,
	InteractionRepeated,
	InteractionSuccess,
	NoInteraction
};

DECLARE_MULTICAST_DELEGATE_OneParam(FInteractionLost, AActor*);
UCLASS()
class ODIN_API UOD_CompInteraction : public UActorComponent
{
	GENERATED_BODY()

public:
	FSimpleMulticastDelegate InteractionAvailable;
	FInteractionLost LostInteraction;
	FSimpleDelegate HoldInteractionStarts;
	FSimpleDelegate HoldInteractionEnds;

	UOD_CompInteraction(const FObjectInitializer& ObjectInitializer);
	void StartInteraction() const;
	void StopInteraction(bool bSucceed) const;
	AActor* GetCurrentInteractActor() const { return CurrentInteractActor.Get(); }
	float GetObjectHoldInteractionTime() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Interact")
		float InteractDistance = 1000.f;

	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;
	TWeakObjectPtr<AActor> CurrentInteractActor = nullptr;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	EOD_InteractionType CanInteractWithAnyObject(FHitResult& OutHit) const;
	bool LineTraceSingle(FHitResult& OutHit) const;
	void ResetInteraction(const AActor* ActorToReset);

};

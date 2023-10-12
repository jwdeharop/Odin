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

UCLASS()
class ODIN_API UOD_CompInteraction : public UActorComponent
{
	GENERATED_BODY()

public:
	FSimpleDelegate InteractionAvailable;
	FSimpleDelegate LostInteraction;

	UOD_CompInteraction(const FObjectInitializer& ObjectInitializer);
	void StartInteraction();
	void StopInteraction();;
	AActor* GetCurrentInteractActor() { return CurrentInteractActor.Get(); }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Interact")
		float InteractDistance = 300.f;

	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;
	TWeakObjectPtr<AActor> CurrentInteractActor = nullptr;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	EOD_InteractionType CanInteractWithAnyObject(FHitResult& OutHit) const;
	bool LineTraceSingle(FHitResult& OutHit) const;
	void ResetInteraction(const AActor* ActorToReset);

};

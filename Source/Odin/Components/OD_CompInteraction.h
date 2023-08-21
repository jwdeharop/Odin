#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OD_CompInteraction.generated.h"

class ACharacter;

UCLASS()
class ODIN_API UOD_CompInteraction : public UActorComponent
{
	GENERATED_BODY()

public:
	UOD_CompInteraction();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Interact")
		float InteractDistance = 300.f;

	TWeakObjectPtr<ACharacter> OwnerCharacter = nullptr;
	TWeakObjectPtr<AActor> CurrentInteractActor = nullptr;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool CanInteractWithAnyObject(FHitResult& OutHit) const;
	bool LineTraceSingle(FHitResult& OutHit) const;
	void ResetInteraction(const AActor* ActorToReset);

};

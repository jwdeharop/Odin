#pragma once

#include "Odin.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OD_InteractionInterface.h"
#include "OD_ElementalExtractionPoint.generated.h"

class UStaticMeshComponent;

UCLASS(config=Game)
class AOD_ElementalExtractionPoint : public AActor, public IOD_InteractionInterface
{
	GENERATED_BODY()
public:
	AOD_ElementalExtractionPoint();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EOD_ElementalDamageType DamageType = EOD_ElementalDamageType::Basic;
	UPROPERTY(EditAnywhere)
		float InteractionTime = 3.f;

	FTimerHandle InteractionTimer;

	UFUNCTION(Client, Reliable)
		void Client_InteractionSuccessful();

	virtual void StartInteraction(ACharacter* InteractionInstigator) override;
	virtual void CancelInteraction() override;
	virtual void InteractionSuccessful() override;
	virtual float GetHoldInteractTime() override;
	virtual void PrepareInteraction(bool bCanInteract) override;

private:
	UPROPERTY(Transient)
		ACharacter* MyInstigator = nullptr;
};


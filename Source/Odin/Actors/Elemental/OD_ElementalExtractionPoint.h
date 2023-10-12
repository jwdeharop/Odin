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

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditAnywhere)
		EOD_ElementalDamageType DamageType = EOD_ElementalDamageType::Basic;

	FTimerHandle InteractionTimer;

	UFUNCTION(Server, Reliable)
		void Server_StartInteraction(ACharacter* InteractionInstigator);
	UFUNCTION(Server, Reliable)
		void Server_StopInteraction();

	virtual void StartInteraction(ACharacter* InteractionInstigator) override;
	virtual void CancelInteraction() override;
	virtual bool IsHoldInteraction() override;
	virtual void InteractionSuccessful() override;

private:
	UPROPERTY(Transient)
		ACharacter* MyInstigator = nullptr;
};


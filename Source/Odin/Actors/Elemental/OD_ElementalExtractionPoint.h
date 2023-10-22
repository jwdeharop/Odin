#pragma once

#include "Odin.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OD_InteractionInterface.h"
#include "PlayerStates/Elemental/OD_ElementalPlayerState.h"
#include "OD_ElementalExtractionPoint.generated.h"

class UStaticMeshComponent;

UCLASS(config=Game)
class AOD_ElementalExtractionPoint : public AActor, public IOD_InteractionInterface
{
	GENERATED_BODY()
public:
	AOD_ElementalExtractionPoint();
	virtual UOD_CompInteractable* GetCompInteractable() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EOD_ElementalDamageType DamageType = EOD_ElementalDamageType::Synthetic;
	UPROPERTY(EditDefaultsOnly)
		UOD_CompInteractable* CompInteractable = nullptr;

	FTimerHandle InteractionTimer;

	virtual void StartInteraction(ACharacter* InteractionInstigator) override;
	virtual void CancelInteraction() override;
	virtual void InteractionSuccessful() override;
	virtual void PrepareInteraction(bool bCanInteract) override;

	virtual void BeginPlay() override;

private:
	UPROPERTY(Transient)
		ACharacter* MyInstigator = nullptr;

	void InteractionStartedOnServer(bool bSucceed);
	void InteractionEndsClient(bool bSucceed);
	void OnClientsStatsChanged(FOD_PlayerStats PlayerStats);
};


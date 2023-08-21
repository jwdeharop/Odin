#pragma once

#include "Odin.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OD_InteractionInterface.h"
#include "OD_PickUpBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UMaterialInstance;

UCLASS()
class ODIN_API AOD_PickUpBase : public AActor, public IOD_InteractionInterface
{
	GENERATED_BODY()

public:
	AOD_PickUpBase();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pick Up")
		EOD_PickUpType PickUpType = EOD_PickUpType::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USphereComponent* InteractCollision = nullptr;
	UPROPERTY(EditDefaultsOnly)
		UMaterialInstance* OverlayMaterial = nullptr;

	virtual void PrepareInteraction(bool bCanInteract) override;

private:
	void SetOverlayMaterial(UMaterialInstance* NewOverlayMaterial);
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OD_InteractionInterface.h"
#include "OD_PickUpBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UMaterialInstance;
class AOD_BaseItem;
struct FOD_InventoryValue;

UCLASS()
class ODIN_API AOD_PickUpBase : public AActor, public IOD_InteractionInterface
{
	GENERATED_BODY()

public:
	AOD_PickUpBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USphereComponent* InteractCollision = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AOD_BaseItem> BaseItem = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 Quantity = 0;

	virtual void StartInteraction() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PrepareInteraction(bool bCanInteract) override;

private:
	void SetOverlayMaterial(UMaterialInstance* NewOverlayMaterial);

};

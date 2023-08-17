#pragma once

#include "Odin.h"
#include "GameFramework/Actor.h"
#include "OD_PickUpBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class ODIN_API AOD_PickUpBase : public AActor
{
	GENERATED_BODY()

public:
	AOD_PickUpBase();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pick Up")
		EOD_PickUpType PickUpType = EOD_PickUpType::None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		UStaticMeshComponent* MeshComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USphereComponent* InteractCollision = nullptr;
};

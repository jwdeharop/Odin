#pragma once

#include "CoreMinimal.h"
#include "Characters/OD_BaseAICharacter.h"
#include "OD_PatrolCharacter.generated.h"

class USplineComponent;

UCLASS()
class ODIN_API AOD_PatrolCharacter : public AOD_BaseAICharacter
{
	GENERATED_BODY()

public:
	AOD_PatrolCharacter();

	void FillSplinePoints();
	const TArray<FVector>& GetSplinePoints() const;
	bool IsUsingTorch() const;

private:
	TArray<FVector> SplinePoints;

protected:
	UPROPERTY(EditInstanceOnly, Category = "AI | Patrol")
		USplineComponent* SplineComponent = nullptr;
	// For now this is set here, will be set by inventory in the future.
	UPROPERTY(EditInstanceOnly, Category = "AI | Patrol")
		bool bIsUsingTorch = false;
	
	virtual void BeginPlay() override;

};

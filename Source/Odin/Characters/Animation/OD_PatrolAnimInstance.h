#pragma once

#include "CoreMinimal.h"
#include "OD_BaseAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "OD_PatrolAnimInstance.generated.h"

UCLASS()
class ODIN_API UOD_PatrolAnimInstance : public UOD_BaseAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Patrol")
		bool bIsUsingTorch = false;

	// Used to gather data.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "OD_BaseAnimInstance.generated.h"

class AOD_BaseCharacter;
class UPawnMovementComponent;

UCLASS()
class ODIN_API UOD_BaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
		bool bIsCrouching = false;
	UPROPERTY(BlueprintReadOnly)
		float GroundSpeed = 0.f;
	UPROPERTY(BlueprintReadOnly)
		FVector Velocity = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly)
		bool bShouldMove = false;
	UPROPERTY(BlueprintReadOnly)
		bool bIsFalling = false;
	UPROPERTY(BlueprintReadOnly)
		bool bIsHoldingItem = false;
	
	TWeakObjectPtr<AOD_BaseCharacter> BaseCharacter;
	TWeakObjectPtr<UPawnMovementComponent> CharacterMovement;

	virtual void NativeBeginPlay() override;
	// Used to gather data.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
